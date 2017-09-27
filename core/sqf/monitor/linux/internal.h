///////////////////////////////////////////////////////////////////////////////
//
// @@@ START COPYRIGHT @@@
//
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
// @@@ END COPYRIGHT @@@
//
///////////////////////////////////////////////////////////////////////////////

#ifndef INTERNAL_H_
#define INTERNAL_H_

enum InternalType
{
      InternalType_Null             // Noop
    , InternalType_ActivateSpare    // activate a spare node
    , InternalType_Clone            // Add clone process to monitor
    , InternalType_Device           // Add or Change device to monitor
    , InternalType_Down             // Node down
    , InternalType_Dump             // Dump process
    , InternalType_DumpComplete     // Dump process complete
    , InternalType_Event            // Send Event to process
    , InternalType_Exit             // Delete process for monitor
    , InternalType_IoData           // Stdin/Stdout data for a process
    , InternalType_Kill             // Kill monitored process
    , InternalType_NodeAdd          // Add Node to configuration database
    , InternalType_NodeAdded        // Reload Node configuration and send node added notice
    , InternalType_NodeDelete       // Delete Node from configuration database
    , InternalType_NodeDeleted      // Reload Node configuration and send node deleted notice
    , InternalType_NodeName         // Node Name Change 
    , InternalType_Notify           // Register for monitoring of process death
    , InternalType_PersistAdd       // Add persist template to configuration database
    , InternalType_PersistDelete    // Delete persist template from configuration database
    , InternalType_Process          // Add process to monitor
    , InternalType_ProcessInit      // Process fork completed
    , InternalType_Open             // Add open to monitor
    , InternalType_Set              // Add or change configuration key
    , InternalType_StdinReq         // Request stdin data via ancestor
    , InternalType_Sync             // Sync monitor for Bcast operation
    , InternalType_Up               // Node up
    , InternalType_CreatePrimitives // Create primitive processes local request
                                    // generated during monitor initialization
                                    // it is never generated by remote monitor
    , InternalType_Quiesce          // Quiesce started
    , InternalType_PostQuiece       // Quiesce ended
    , InternalType_Revive          // Revive request
    , InternalType_Snapshot         // Snapshot request
    , InternalType_UniqStr          // Unique string
    , InternalType_TmReady          // DTM ready for transactions
    , InternalType_Shutdown         // Shutdown
    , InternalType_SchedData        // Processor & memory statistics
    , InternalType_SoftNodeDown     // Soft Node down (SQWatchdog not triggered)
    , InternalType_SoftNodeUp       // Soft Node up
};

enum SyncType
{
    SyncType_NULL,                  // No sync requested
    SyncType_TmData,                // sync to exchange TM data amoung TMs
    SyncType_TmSyncState            // sync the current TmSync state across the cluster
};

enum SyncState
{
    SyncState_Null,                 // No sync active
    SyncState_Start,                // Sync request started
    SyncState_Continue,             // Continue with sync on remote node
    SyncState_Abort,                // Sync Abort data
    SyncState_Commit,               // Sync Commit data
    SyncState_Suspended             // Sync Suspended (until spare goes active)
};

typedef enum {
    State_Default=0,
    State_Quiesce,                  // node quiesce state while going down
    State_SoftDown,                 // node soft down on DTM abort -> restart
    State_Ready_To_Exit
} IntNodeState; 

typedef unsigned long long upNode_t;
typedef struct 
{
    upNode_t upNodes[MAX_NODE_MASKS];
} upNodes_t;

typedef int Verifier_t;             // Process verifier typedef

struct clone_def
{
    bool backup;           // true for a backup process
    bool unhooked;
    bool event_messages;   // true if wants to receive notices
    bool system_messages;  // true if wants to system messages
    int nid;               // node id of process
    Verifier_t verifier;   // verifier of the process
    PROCESSTYPE type;      // process handling catagory
    int priority;          // process priority
    int parent_nid;        // node id of parent process
    int parent_pid;        // process id of parent process
    Verifier_t parent_verifier; // verifier of parent process
    int os_pid;            // native OS process id
    pid_t prior_pid;       // for restarted persistent process, the
                           // previous process id.  Otherwise zero.
    bool persistent;       // persistent process or not
    int persistent_retries;   // number of times process has been restarted

    int  argc;             // number of command line arguments
    struct timespec creation_time;      // process creation time

    strId_t pathStrId;     // program lookup path (string id)
    strId_t ldpathStrId;   // library load path (string id)
    strId_t programStrId;  // full path to object file (string id)

    int  nameLen;
    int  portLen;
    int  argvLen;
    int  infileLen;
    int  outfileLen;
    char stringData;       // variable length string data
};

struct device_def
{
    char ldev_name[MAX_KEY_NAME];   // Logical device name
    bool primary_mounted;           // phyical primary device's mount state
    bool mirror_mounted;            // phyical mirror device's mount state
};

struct down_def
{
    int pnid;                       // Physical node id
};

struct shutdown_def
{
    int level;                      // shutdown level
};

struct dump_def
{
    int        nid;                         // Node id of process to dump
    int        pid;                         // Process id of process to dump
    Verifier_t verifier;                    // verifier of the process to dump
    int        dumper_nid;                  // Requesting process's node id
    int        dumper_pid;                  // Requesting process's process id
    Verifier_t dumper_verifier;             // Requesting process's verifier
    char       core_file[MAX_PROCESS_PATH]; // target core path
    DUMPSTATUS status;                      // completion status
};

struct exit_def
{
    int nid;                        // Node id of process exiting
    int pid;                        // Process id of process exiting
    Verifier_t verifier;            // Verifier of the process exiting
    char name[MAX_PROCESS_NAME];    // Name of process exiting
    bool abended;
};

struct event_def
{
    int  nid;                        // Nid id of process to receive event
    int  pid;                        // Process id of process to receive event
    Verifier_t verifier;             // Verifier of the process to receive event
    int  event_id;                   // Event id to send to nid,pid
    int  length;                     // Length in byte of Data buffer used
    char data;                       // Data to be replicated across cluster
};

enum StdIoType
{
    STDIN_DATA,
    STDOUT_DATA
};

typedef struct
{
    int  nid;                        // Nid id of process to receive io data
    int  pid;                        // Process id of process to receive io data
    StdIoType   ioType;
    int  length;                     // Length in byte of Data buffer used
    char data[MAX_SYNC_DATA];        // Data to be replicated across cluster
} ioData_t;

enum StdinReqType
{
    STDIN_REQ_DATA,
    STDIN_FLOW_OFF,
    STDIN_FLOW_ON
};

struct stdin_req_def
{
    int nid;                         // Node id of process requesting stdin data
    int pid;                         // Process id of process requesting stdin data
    StdinReqType reqType;
    int supplier_nid;                // Node id of process supplying stdin data
    int supplier_pid;              // Process id of process to supplying stdin data
};

struct kill_def
{
    int nid;                // Node id of process to kill
    int pid;                // Process id of process to kill
    Verifier_t verifier;    // Verifier of the process to kill
    bool persistent_abort;  // when true, persistent process is not restarted
                            // otherwise, it is ignored
};

struct node_add_def
{
    int req_nid;                            // Node id of requesting process
    int req_pid;                            // Process id of requesting process
    Verifier_t req_verifier;                // Verifier of the requesting process
    char node_name[MPI_MAX_PROCESSOR_NAME]; // Node's name
    int  first_core;                        // First or only core assigned
    int  last_core;                         // Last core assigned or -1
    int  processors;                        // Number of processors in logical node
    int  roles;                             // Role assigment
};

struct node_added_def
{
    int  pnid;                              // Target pnid
};

struct node_delete_def
{
    int req_nid;                            // Node id of requesting process
    int req_pid;                            // Process id of requesting process
    Verifier_t req_verifier;                // Verifier of the requesting process
    int  pnid;                              // Target pnid
};

struct node_deleted_def
{
    int  pnid;                              // Target pnid
};

struct node_name_def
{
    int req_nid;                            // Node id of requesting process
    int req_pid;                            // Process id of requesting process
    Verifier_t req_verifier;                // Verifier of the requesting process
    char current_name[MPI_MAX_PROCESSOR_NAME];
    char new_name[MPI_MAX_PROCESSOR_NAME];
};


struct notify_def
{
    int nid;            // Node id of process being notified
    int pid;            // Process id of process being notified
    int canceled;                   // If true,  notice request has been canceled
    int target_nid;         // Node id of process being monitored
    int target_pid;         // Process id of process being monitored
    _TM_Txid_External trans_id;             // Associated TransID
};


struct process_def
{
    bool backup;           // true for a backup process
    bool unhooked;
    int nid;               // node id of process
    int pid;               // process id
    PROCESSTYPE type;      // process handling catagory
    int priority;          // process priority
    int parent_nid;        // node id of parent process
    int parent_pid;        // process id of parent process
    Verifier_t parent_verifier; // process id of parent process
    int pair_parent_nid;   // node id of real process pair parent process
    int pair_parent_pid;   // process id of real process pair parent process
    Verifier_t pair_parent_verifier; // process id of real process pair parent process
    int  argc;             // number of command line arguments

    strId_t pathStrId;     // program lookup path (string id)
    strId_t ldpathStrId;   // library load path (string id)
    strId_t programStrId;  // full path to object file (string id)

    void *tag;             // process tag
    int  nameLen;
    int  argvLen;
    int  infileLen;
    int  outfileLen;
    char stringData;       // variable length string data
};



struct process_init_def
{
    int nid;             // Node id of child process
    int pid;             // Process id of child process
    Verifier_t verifier; // Verifier of child process
    STATE state;
    int origNid;         // Node id where the "new process" request originated
    int result;          // zero if process created, error code otherwise
    void *tag;           // process tag sent with original "process" replication
    char name[MAX_PROCESS_NAME];   // Name of child process
};

struct open_def
{
    int nid;                            // Node id of process performing open
    int pid;                            // Process id of process performing open
    Verifier_t verifier;                // verifier of the process performing open
    int opened_nid;                     // Node id of process to be opened
    int opened_pid;                     // Process id of process to be opened
    Verifier_t opened_verifier;         // verifier of the process to be opened
};

// Processor status for a given core
typedef struct
{
    long long  cpu_user;            // Time in user mode
    long long  cpu_nice;            // Time in user mode, low priority
    long long  cpu_system;          // Time in system mode
    long long  cpu_idle;            // Time in idle task
    long long  cpu_iowait;          // Time waiting for i/o
    long long  cpu_irq;             // Time in hardware interrupt
    long long  cpu_soft_irq;        // Time in software interrupt
} ProcStat_t;

struct scheddata_def
{
    int PNid;                           // Node ID of scheduling data
    int processors;                     // Number of processors in node
    unsigned int memory_total;          // Node's total memory
    unsigned int memory_free;           // Node's current free memory
    unsigned int swap_free;             // Node's current free swap
    unsigned int cache_free;            // Node's current free buffer/cache
    unsigned int memory_active;         // Node's memory in active use
    unsigned int memory_inactive;       // Node's memory available for reclamation
    unsigned int memory_dirty;          // Node's memory waiting to be written to disk
    unsigned int memory_writeback;      // Node's memory being written to disk
    unsigned int memory_VMallocUsed;    // Node's amount of used virtual memory
    ProcStat_t   proc_stats[MAX_LNODES_PER_NODE]; // Per logical node processor statistics
    unsigned int btime;                 // Node boot time
};


struct set_def
{
    ConfigType type;                    // type of group being set
    char group[MAX_KEY_NAME];           // if type==ConfigType_Process then group's name.
    char key[MAX_KEY_NAME];             // key name of the item being set
    char valueData;       // variable length string data
};


struct spare_def
{
    int down_pnid;                       // down node physical node id
    int spare_pnid;                      // spare node physical node id
};

struct sync_def
{
    SyncType  type;                      // Coordination type
    int       pnid;                      // My Physical Node id
    int       syncnid;                   // TM's Logical Node driving the sync
    int       tmleader;                  // TM Leader's Logical Node id
    SyncState state;                     // My current TM sync state 
    int       count;                     // # of requests in data
    int       length;                    // data buffer length
    char      data[MAX_SYNC_DATA];       // Length/Data pairs to by replicated across cluster
};

struct up_def
{
    int pnid;                       // Physical node id
};

struct uniqstr_def
{
    int nid;
    int id;
    char valueData;       // variable length string data
};

struct internal_msg_def
{
    InternalType type;
    int replSize;
    union 
    {
        struct clone_def   clone;
        struct device_def  device;
        struct down_def    down;
        struct dump_def    dump;
        struct exit_def    exit;
        struct event_def   event;
        ioData_t           iodata;
        struct kill_def    kill;
        struct node_add_def     node_add;
        struct node_added_def   node_added;
        struct node_delete_def  node_delete;
        struct node_deleted_def node_deleted;
        struct node_name_def    node_name;
        struct notify_def  notify;
        struct process_def process;
        struct process_init_def processInit;
        struct open_def    open;
        struct set_def     set;
        struct stdin_req_def stdin_req;
        struct sync_def    sync;
        struct up_def      up;
        struct spare_def   activate_spare;
        struct uniqstr_def uniqstr;
        struct shutdown_def shutdown;
        struct scheddata_def scheddata;
    } u;
} __attribute__((__may_alias__));

// Define a constant giving the "header" size of the internal_msg_def above
#define MSG_HDR_SIZE  ( sizeof (InternalType) + sizeof (int) )


typedef struct nodeId_s
{
    char nodeName[MPI_MAX_PROCESSOR_NAME];
    char commPort[MPI_MAX_PORT_NAME];
    char syncPort[MPI_MAX_PORT_NAME];
    int  pnid;
    int  creatorPNid;
    int  creatorShellPid;
    Verifier_t creatorShellVerifier;
    bool creator;  // NEW monitor set to true to tell creator it is the CREATOR
    bool ping;     // Monitor sets to true to tell remote monitor
                   // it is just checking that it can communicate with it.
                   // Used during allgather reconnect 
} nodeId_t;

typedef struct nodeStatus_s
{
    STATE state;
    int   status;
} nodeStatus_t;

typedef struct cluster_state_def
{
    char node_state;
    IntNodeState internalState;         // internal state of a node
    SyncState tmSyncState;              // Node's current tm sync process state
    ShutdownLevel sdLevel;              // Node's current shutdown level
    int change_nid;
    unsigned long long seq_num;
    upNodes_t nodeMask;                 // Set of nodes currently "up"
} cluster_state_def_t;

typedef struct msgInfo
{
    int msg_count;                      // Number if messages to replicate
    int msg_offset;                     // Current offset into the msg buffer
} msgInfo_t;

struct sync_buffer_def
{
    cluster_state_def_t nodeInfo;
    msgInfo_t msgInfo;
    char msg[MAX_SYNC_SIZE];
};


#endif
