#include "app_main.h"

#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME          {6,'T','E','L','I','N','K'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID          {8,'T','L','S','R','8','2','x','x'}
#endif
#ifndef ZCL_BASIC_SW_BUILD_ID
#define ZCL_BASIC_SW_BUILD_ID       {10,'0','1','2','2','0','5','2','0','1','7'}
#endif

#define R               ACCESS_CONTROL_READ
#define RW              ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE
#define RR              ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE
#define RWR             ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE | ACCESS_CONTROL_REPORTABLE

#define ZCL_UINT8       ZCL_DATA_TYPE_UINT8
#define ZCL_INT8        ZCL_DATA_TYPE_INT8
#define ZCL_UINT16      ZCL_DATA_TYPE_UINT16
#define ZCL_INT16       ZCL_DATA_TYPE_INT16
#define ZCL_UINT32      ZCL_DATA_TYPE_UINT32
#define ZCL_ENUM8       ZCL_DATA_TYPE_ENUM8
#define ZCL_ENUM16		ZCL_DATA_TYPE_ENUM16
#define ZCL_BOOLEAN     ZCL_DATA_TYPE_BOOLEAN
#define ZCL_BITMAP8     ZCL_DATA_TYPE_BITMAP8
#define ZCL_BITMAP16    ZCL_DATA_TYPE_BITMAP16
#define ZCL_CHAR_STR    ZCL_DATA_TYPE_CHAR_STR
#define ZCL_UTC         ZCL_DATA_TYPE_UTC
//#define ZCL_SINGLE      ZCL_DATA_TYPE_SINGLE_PREC
#define ZCL_IEEE_ADDR   ZCL_DATA_TYPE_IEEE_ADDR

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const uint16_t app_ep1_inClusterList[] =
{
    ZCL_CLUSTER_GEN_BASIC,
    ZCL_CLUSTER_GEN_IDENTIFY,
    ZCL_CLUSTER_GEN_POWER_CFG,
    ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
    ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const uint16_t app_ep1_outClusterList[] =
{
    ZCL_CLUSTER_GEN_GROUPS,
    ZCL_CLUSTER_GEN_SCENES,
    ZCL_CLUSTER_OTA,
#ifdef ZCL_ZLL_COMMISSIONING
    ZCL_CLUSTER_TOUCHLINK_COMMISSIONING,
#endif
    ZCL_CLUSTER_GEN_ON_OFF,
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const uint16_t app_ep2_inClusterList[] =
{
    ZCL_CLUSTER_GEN_IDENTIFY,
    ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
    ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const uint16_t app_ep2_outClusterList[] =
{
    ZCL_CLUSTER_GEN_GROUPS,
    ZCL_CLUSTER_GEN_SCENES,
    ZCL_CLUSTER_GEN_ON_OFF,
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const uint16_t app_ep3_inClusterList[] =
{
    ZCL_CLUSTER_GEN_IDENTIFY,
    ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
    ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const uint16_t app_ep3_outClusterList[] =
{
    ZCL_CLUSTER_GEN_GROUPS,
    ZCL_CLUSTER_GEN_SCENES,
    ZCL_CLUSTER_GEN_ON_OFF,
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const uint16_t app_ep4_inClusterList[] =
{
    ZCL_CLUSTER_GEN_IDENTIFY,
    ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
    ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const uint16_t app_ep4_outClusterList[] =
{
    ZCL_CLUSTER_GEN_GROUPS,
    ZCL_CLUSTER_GEN_SCENES,
    ZCL_CLUSTER_GEN_ON_OFF,
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const uint16_t app_ep5_inClusterList[] =
{
    ZCL_CLUSTER_GEN_IDENTIFY,
    ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
    ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const uint16_t app_ep5_outClusterList[] =
{
    ZCL_CLUSTER_GEN_GROUPS,
    ZCL_CLUSTER_GEN_SCENES,
    ZCL_CLUSTER_GEN_ON_OFF,
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const uint16_t app_ep6_inClusterList[] =
{
    ZCL_CLUSTER_GEN_IDENTIFY,
    ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
    ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const uint16_t app_ep6_outClusterList[] =
{
    ZCL_CLUSTER_GEN_GROUPS,
    ZCL_CLUSTER_GEN_SCENES,
    ZCL_CLUSTER_GEN_ON_OFF,
    ZCL_CLUSTER_GEN_LEVEL_CONTROL,
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define APP_EP1_IN_CLUSTER_NUM   (sizeof(app_ep1_inClusterList)/sizeof(app_ep1_inClusterList[0]))
#define APP_EP1_OUT_CLUSTER_NUM  (sizeof(app_ep1_outClusterList)/sizeof(app_ep1_outClusterList[0]))
#define APP_EP2_IN_CLUSTER_NUM   (sizeof(app_ep2_inClusterList)/sizeof(app_ep2_inClusterList[0]))
#define APP_EP2_OUT_CLUSTER_NUM  (sizeof(app_ep2_outClusterList)/sizeof(app_ep2_outClusterList[0]))
#define APP_EP3_IN_CLUSTER_NUM   (sizeof(app_ep3_inClusterList)/sizeof(app_ep3_inClusterList[0]))
#define APP_EP3_OUT_CLUSTER_NUM  (sizeof(app_ep3_outClusterList)/sizeof(app_ep3_outClusterList[0]))
#define APP_EP4_IN_CLUSTER_NUM   (sizeof(app_ep4_inClusterList)/sizeof(app_ep4_inClusterList[0]))
#define APP_EP4_OUT_CLUSTER_NUM  (sizeof(app_ep4_outClusterList)/sizeof(app_ep4_outClusterList[0]))
#define APP_EP5_IN_CLUSTER_NUM   (sizeof(app_ep5_inClusterList)/sizeof(app_ep5_inClusterList[0]))
#define APP_EP5_OUT_CLUSTER_NUM  (sizeof(app_ep5_outClusterList)/sizeof(app_ep5_outClusterList[0]))
#define APP_EP6_IN_CLUSTER_NUM   (sizeof(app_ep6_inClusterList)/sizeof(app_ep6_inClusterList[0]))
#define APP_EP6_OUT_CLUSTER_NUM  (sizeof(app_ep6_outClusterList)/sizeof(app_ep6_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t app_ep1Desc = {
    HA_PROFILE_ID,                          /* Application profile identifier */
    HA_DEV_ONOFF_SWITCH,                    /* Application device identifier */
    APP_ENDPOINT1,                          /* Endpoint */
    2,                                      /* Application device version */
    0,                                      /* Reserved */
    APP_EP1_IN_CLUSTER_NUM,                 /* Application input cluster count */
    APP_EP1_OUT_CLUSTER_NUM,                /* Application output cluster count */
    (uint16_t *)app_ep1_inClusterList,      /* Application input cluster list */
    (uint16_t *)app_ep1_outClusterList,     /* Application output cluster list */
};

const af_simple_descriptor_t app_ep2Desc = {
    HA_PROFILE_ID,                          /* Application profile identifier */
    HA_DEV_ONOFF_SWITCH,                    /* Application device identifier */
    APP_ENDPOINT2,                          /* Endpoint */
    2,                                      /* Application device version */
    0,                                      /* Reserved */
    APP_EP2_IN_CLUSTER_NUM,                 /* Application input cluster count */
    APP_EP2_OUT_CLUSTER_NUM,                /* Application output cluster count */
    (uint16_t *)app_ep2_inClusterList,      /* Application input cluster list */
    (uint16_t *)app_ep2_outClusterList,     /* Application output cluster list */
};

const af_simple_descriptor_t app_ep3Desc = {
    HA_PROFILE_ID,                          /* Application profile identifier */
    HA_DEV_ONOFF_SWITCH,                    /* Application device identifier */
    APP_ENDPOINT3,                          /* Endpoint */
    2,                                      /* Application device version */
    0,                                      /* Reserved */
    APP_EP3_IN_CLUSTER_NUM,                 /* Application input cluster count */
    APP_EP3_OUT_CLUSTER_NUM,                /* Application output cluster count */
    (uint16_t *)app_ep3_inClusterList,      /* Application input cluster list */
    (uint16_t *)app_ep3_outClusterList,     /* Application output cluster list */
};

const af_simple_descriptor_t app_ep4Desc = {
    HA_PROFILE_ID,                          /* Application profile identifier */
    HA_DEV_ONOFF_SWITCH,                    /* Application device identifier */
    APP_ENDPOINT4,                          /* Endpoint */
    2,                                      /* Application device version */
    0,                                      /* Reserved */
    APP_EP4_IN_CLUSTER_NUM,                 /* Application input cluster count */
    APP_EP4_OUT_CLUSTER_NUM,                /* Application output cluster count */
    (uint16_t *)app_ep4_inClusterList,      /* Application input cluster list */
    (uint16_t *)app_ep4_outClusterList,     /* Application output cluster list */
};

const af_simple_descriptor_t app_ep5Desc = {
    HA_PROFILE_ID,                          /* Application profile identifier */
    HA_DEV_ONOFF_SWITCH,                    /* Application device identifier */
    APP_ENDPOINT5,                          /* Endpoint */
    2,                                      /* Application device version */
    0,                                      /* Reserved */
    APP_EP5_IN_CLUSTER_NUM,                 /* Application input cluster count */
    APP_EP5_OUT_CLUSTER_NUM,                /* Application output cluster count */
    (uint16_t *)app_ep5_inClusterList,      /* Application input cluster list */
    (uint16_t *)app_ep5_outClusterList,     /* Application output cluster list */
};

const af_simple_descriptor_t app_ep6Desc = {
    HA_PROFILE_ID,                          /* Application profile identifier */
    HA_DEV_ONOFF_SWITCH,                    /* Application device identifier */
    APP_ENDPOINT6,                          /* Endpoint */
    2,                                      /* Application device version */
    0,                                      /* Reserved */
    APP_EP6_IN_CLUSTER_NUM,                 /* Application input cluster count */
    APP_EP6_OUT_CLUSTER_NUM,                /* Application output cluster count */
    (uint16_t *)app_ep6_inClusterList,      /* Application input cluster list */
    (uint16_t *)app_ep6_outClusterList,     /* Application output cluster list */
};


/* Basic */
zcl_basicAttr_t g_zcl_basicAttrs =
{
    .zclVersion     = 0x03,
    .appVersion     = APP_RELEASE,
    .stackVersion   = (STACK_RELEASE|STACK_BUILD),
    .hwVersion      = HW_VERSION,
    .manuName       = ZCL_BASIC_MFG_NAME,
    .modelId        = ZCL_BASIC_MODEL_ID,
    .dateCode       = ZCL_BASIC_DATE_CODE,
    .powerSource    = POWER_SOURCE_BATTERY,
    .swBuildId      = ZCL_BASIC_SW_BUILD_ID,
    .deviceEnable   = TRUE,
};

const zclAttrInfo_t basic_attrTbl[] =
{
    { ZCL_ATTRID_BASIC_ZCL_VER,             ZCL_UINT8,    R,  (uint8_t*)&g_zcl_basicAttrs.zclVersion},
    { ZCL_ATTRID_BASIC_APP_VER,             ZCL_UINT8,    R,  (uint8_t*)&g_zcl_basicAttrs.appVersion},
    { ZCL_ATTRID_BASIC_STACK_VER,           ZCL_UINT8,    R,  (uint8_t*)&g_zcl_basicAttrs.stackVersion},
    { ZCL_ATTRID_BASIC_HW_VER,              ZCL_UINT8,    R,  (uint8_t*)&g_zcl_basicAttrs.hwVersion},
    { ZCL_ATTRID_BASIC_MFR_NAME,            ZCL_CHAR_STR, R,  (uint8_t*)g_zcl_basicAttrs.manuName},
    { ZCL_ATTRID_BASIC_MODEL_ID,            ZCL_CHAR_STR, R,  (uint8_t*)g_zcl_basicAttrs.modelId},
    { ZCL_ATTRID_BASIC_DATE_CODE,           ZCL_CHAR_STR, R,  (uint8_t*)g_zcl_basicAttrs.dateCode},
    { ZCL_ATTRID_BASIC_POWER_SOURCE,        ZCL_ENUM8,    R,  (uint8_t*)&g_zcl_basicAttrs.powerSource},
    { ZCL_ATTRID_BASIC_DEV_ENABLED,         ZCL_BOOLEAN,  RW, (uint8_t*)&g_zcl_basicAttrs.deviceEnable},
    { ZCL_ATTRID_BASIC_SW_BUILD_ID,         ZCL_CHAR_STR, R,  (uint8_t*)&g_zcl_basicAttrs.swBuildId},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (uint8_t*)&zcl_attr_global_clusterRevision},

};

#define ZCL_BASIC_ATTR_NUM       sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)


/* Identify */
zcl_identifyAttr_t g_zcl_identifyAttrs =
{
    .identifyTime   = 0x0000,
};

const zclAttrInfo_t identify_attrTbl[] =
{
    { ZCL_ATTRID_IDENTIFY_TIME,             ZCL_UINT16,   RW,   (uint8_t*)&g_zcl_identifyAttrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,    (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_IDENTIFY_ATTR_NUM           sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)

zcl_identifyAttr_t g_zcl_identify2Attrs =
{
    .identifyTime   = 0x0000,
};

const zclAttrInfo_t identify2_attrTbl[] =
{
    { ZCL_ATTRID_IDENTIFY_TIME,             ZCL_UINT16,   RW,   (uint8_t*)&g_zcl_identify2Attrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,    (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_IDENTIFY2_ATTR_NUM           sizeof(identify2_attrTbl) / sizeof(zclAttrInfo_t)

zcl_identifyAttr_t g_zcl_identify3Attrs =
{
    .identifyTime   = 0x0000,
};

const zclAttrInfo_t identify3_attrTbl[] =
{
    { ZCL_ATTRID_IDENTIFY_TIME,             ZCL_UINT16,   RW,   (uint8_t*)&g_zcl_identify3Attrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,    (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_IDENTIFY3_ATTR_NUM           sizeof(identify3_attrTbl) / sizeof(zclAttrInfo_t)

zcl_identifyAttr_t g_zcl_identify4Attrs =
{
    .identifyTime   = 0x0000,
};

const zclAttrInfo_t identify4_attrTbl[] =
{
    { ZCL_ATTRID_IDENTIFY_TIME,             ZCL_UINT16,   RW,   (uint8_t*)&g_zcl_identify4Attrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,    (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_IDENTIFY4_ATTR_NUM           sizeof(identify4_attrTbl) / sizeof(zclAttrInfo_t)

zcl_identifyAttr_t g_zcl_identify5Attrs =
{
    .identifyTime   = 0x0000,
};

const zclAttrInfo_t identify5_attrTbl[] =
{
    { ZCL_ATTRID_IDENTIFY_TIME,             ZCL_UINT16,   RW,   (uint8_t*)&g_zcl_identify5Attrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,    (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_IDENTIFY5_ATTR_NUM           sizeof(identify5_attrTbl) / sizeof(zclAttrInfo_t)

zcl_identifyAttr_t g_zcl_identify6Attrs =
{
    .identifyTime   = 0x0000,
};

const zclAttrInfo_t identify6_attrTbl[] =
{
    { ZCL_ATTRID_IDENTIFY_TIME,             ZCL_UINT16,   RW,   (uint8_t*)&g_zcl_identify6Attrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,    (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_IDENTIFY6_ATTR_NUM           sizeof(identify6_attrTbl) / sizeof(zclAttrInfo_t)

/* power */
zcl_powerAttr_t g_zcl_powerAttrs =
{
    .batteryPercentage = 0x64, //in 0,5% units, 0xff - unknown
};

const zclAttrInfo_t powerCfg_attrTbl[] =
{
    { ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING,  ZCL_UINT8,  RR, (uint8_t*)&g_zcl_powerAttrs.batteryPercentage},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,       ZCL_UINT16, R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_POWER_CFG_ATTR_NUM       sizeof(powerCfg_attrTbl) / sizeof(zclAttrInfo_t)

zcl_msInputAttr_t g_zcl_msInputAttrs[DEVICE_BUTTON_MAX] = {
    {
        .value = ACTION_EMPTY,
        .num = 8,
        .out_of_service = 0,
        .status_flag = 0,
    },
    {
        .value = ACTION_EMPTY,
        .num = 8,
        .out_of_service = 0,
        .status_flag = 0,
    },
    {
        .value = ACTION_EMPTY,
        .num = 8,
        .out_of_service = 0,
        .status_flag = 0,
    },
    {
        .value = ACTION_EMPTY,
        .num = 8,
        .out_of_service = 0,
        .status_flag = 0,
    },
    {
        .value = ACTION_EMPTY,
        .num = 8,
        .out_of_service = 0,
        .status_flag = 0,
    },
    {
        .value = ACTION_EMPTY,
        .num = 8,
        .out_of_service = 0,
        .status_flag = 0,
    },
};

const zclAttrInfo_t msInput1_attrTbl[] = {
        { ZCL_MULTISTATE_INPUT_ATTRID_OUT_OF_SERVICE,   ZCL_BOOLEAN,    RW,     (uint8_t*)&g_zcl_msInputAttrs[0].out_of_service },
        { ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE,    ZCL_UINT16,     RWR,    (uint8_t*)&g_zcl_msInputAttrs[0].value          },
        { ZCL_MULTISTATE_INPUT_ATTRID_STATUS_FLAGS,     ZCL_BITMAP8,    RR,     (uint8_t*)&g_zcl_msInputAttrs[0].status_flag    },
        { ZCL_MULTISTATE_INPUT_ATTRID_NUM_OF_STATES,    ZCL_UINT16,     R,      (uint8_t*)&g_zcl_msInputAttrs[0].num            },

        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision      },

};

#define ZCL_MSINPUT1_ATTR_NUM   sizeof(msInput1_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t msInput2_attrTbl[] = {
        { ZCL_MULTISTATE_INPUT_ATTRID_OUT_OF_SERVICE,   ZCL_BOOLEAN,    RW,     (uint8_t*)&g_zcl_msInputAttrs[1].out_of_service },
        { ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE,    ZCL_UINT16,     RWR,    (uint8_t*)&g_zcl_msInputAttrs[1].value          },
        { ZCL_MULTISTATE_INPUT_ATTRID_STATUS_FLAGS,     ZCL_BITMAP8,    RR,     (uint8_t*)&g_zcl_msInputAttrs[1].status_flag    },
        { ZCL_MULTISTATE_INPUT_ATTRID_NUM_OF_STATES,    ZCL_UINT16,     R,      (uint8_t*)&g_zcl_msInputAttrs[1].num            },

        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision      },

};

#define ZCL_MSINPUT2_ATTR_NUM   sizeof(msInput2_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t msInput3_attrTbl[] = {
        { ZCL_MULTISTATE_INPUT_ATTRID_OUT_OF_SERVICE,   ZCL_BOOLEAN,    RW,     (uint8_t*)&g_zcl_msInputAttrs[2].out_of_service },
        { ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE,    ZCL_UINT16,     RWR,    (uint8_t*)&g_zcl_msInputAttrs[2].value          },
        { ZCL_MULTISTATE_INPUT_ATTRID_STATUS_FLAGS,     ZCL_BITMAP8,    RR,     (uint8_t*)&g_zcl_msInputAttrs[2].status_flag    },
        { ZCL_MULTISTATE_INPUT_ATTRID_NUM_OF_STATES,    ZCL_UINT16,     R,      (uint8_t*)&g_zcl_msInputAttrs[2].num            },

        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision      },

};

#define ZCL_MSINPUT3_ATTR_NUM   sizeof(msInput3_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t msInput4_attrTbl[] = {
        { ZCL_MULTISTATE_INPUT_ATTRID_OUT_OF_SERVICE,   ZCL_BOOLEAN,    RW,     (uint8_t*)&g_zcl_msInputAttrs[3].out_of_service },
        { ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE,    ZCL_UINT16,     RWR,    (uint8_t*)&g_zcl_msInputAttrs[3].value          },
        { ZCL_MULTISTATE_INPUT_ATTRID_STATUS_FLAGS,     ZCL_BITMAP8,    RR,     (uint8_t*)&g_zcl_msInputAttrs[3].status_flag    },
        { ZCL_MULTISTATE_INPUT_ATTRID_NUM_OF_STATES,    ZCL_UINT16,     R,      (uint8_t*)&g_zcl_msInputAttrs[3].num            },

        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision      },

};

#define ZCL_MSINPUT4_ATTR_NUM   sizeof(msInput4_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t msInput5_attrTbl[] = {
        { ZCL_MULTISTATE_INPUT_ATTRID_OUT_OF_SERVICE,   ZCL_BOOLEAN,    RW,     (uint8_t*)&g_zcl_msInputAttrs[4].out_of_service },
        { ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE,    ZCL_UINT16,     RWR,    (uint8_t*)&g_zcl_msInputAttrs[4].value          },
        { ZCL_MULTISTATE_INPUT_ATTRID_STATUS_FLAGS,     ZCL_BITMAP8,    RR,     (uint8_t*)&g_zcl_msInputAttrs[4].status_flag    },
        { ZCL_MULTISTATE_INPUT_ATTRID_NUM_OF_STATES,    ZCL_UINT16,     R,      (uint8_t*)&g_zcl_msInputAttrs[4].num            },

        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision      },

};

#define ZCL_MSINPUT5_ATTR_NUM   sizeof(msInput5_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t msInput6_attrTbl[] = {
        { ZCL_MULTISTATE_INPUT_ATTRID_OUT_OF_SERVICE,   ZCL_BOOLEAN,    RW,     (uint8_t*)&g_zcl_msInputAttrs[5].out_of_service },
        { ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE,    ZCL_UINT16,     RWR,    (uint8_t*)&g_zcl_msInputAttrs[5].value          },
        { ZCL_MULTISTATE_INPUT_ATTRID_STATUS_FLAGS,     ZCL_BITMAP8,    RR,     (uint8_t*)&g_zcl_msInputAttrs[5].status_flag    },
        { ZCL_MULTISTATE_INPUT_ATTRID_NUM_OF_STATES,    ZCL_UINT16,     R,      (uint8_t*)&g_zcl_msInputAttrs[5].num            },

        { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,           ZCL_UINT16,     R,      (uint8_t*)&zcl_attr_global_clusterRevision      },

};

#define ZCL_MSINPUT6_ATTR_NUM   sizeof(msInput6_attrTbl) / sizeof(zclAttrInfo_t)

#ifdef ZCL_GROUP
/* Group */
zcl_groupAttr_t g_zcl_groupAttrs[DEVICE_BUTTON_MAX] =
{
    {.nameSupport = 0},
    {.nameSupport = 0},
    {.nameSupport = 0},
    {.nameSupport = 0},
    {.nameSupport = 0},
    {.nameSupport = 0},
};

const zclAttrInfo_t group1_attrTbl[] =
{
    { ZCL_ATTRID_GROUP_NAME_SUPPORT,        ZCL_BITMAP8,    R,  (uint8_t*)&g_zcl_groupAttrs[0].nameSupport     },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision  },
};

#define ZCL_GROUP1_ATTR_NUM    sizeof(group1_attrTbl) / sizeof(zclAttrInfo_t)

#endif

#ifdef ZCL_SCENE
/* Scene */
zcl_sceneAttr_t g_zcl_sceneAttrs[DEVICE_BUTTON_MAX] =
{
    {
        .sceneCount     = 0,
        .currentScene   = 0,
        .currentGroup   = 0x0000,
        .sceneValid     = FALSE,
        .nameSupport    = 0,
        .customScene    = 0,
        .customGroup    = 0,
    },
};

const zclAttrInfo_t scene1_attrTbl[] = {
    { ZCL_ATTRID_SCENE_SCENE_COUNT,         ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[0].sceneCount     },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,       ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[0].currentScene   },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,       ZCL_UINT16,   R,  (uint8_t*)&g_zcl_sceneAttrs[0].currentGroup   },
    { ZCL_ATTRID_SCENE_SCENE_VALID,         ZCL_BOOLEAN,  R,  (uint8_t*)&g_zcl_sceneAttrs[0].sceneValid     },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,        ZCL_BITMAP8,  R,  (uint8_t*)&g_zcl_sceneAttrs[0].nameSupport    },
    { ZCL_CUSTOM_ATTRID_SCENE_SCENE_ID,     ZCL_UINT8,    RW, (uint8_t*)&g_zcl_sceneAttrs[0].customScene    },
    { ZCL_CUSTOM_ATTRID_SCENE_GROUP_ID,     ZCL_UINT16,   RW, (uint8_t*)&g_zcl_sceneAttrs[0].customGroup    },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE1_ATTR_NUM   sizeof(scene1_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t scene2_attrTbl[] = {
    { ZCL_ATTRID_SCENE_SCENE_COUNT,         ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[1].sceneCount     },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,       ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[1].currentScene   },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,       ZCL_UINT16,   R,  (uint8_t*)&g_zcl_sceneAttrs[1].currentGroup   },
    { ZCL_ATTRID_SCENE_SCENE_VALID,         ZCL_BOOLEAN,  R,  (uint8_t*)&g_zcl_sceneAttrs[1].sceneValid     },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,        ZCL_BITMAP8,  R,  (uint8_t*)&g_zcl_sceneAttrs[1].nameSupport    },
    { ZCL_CUSTOM_ATTRID_SCENE_SCENE_ID,     ZCL_UINT8,    RW, (uint8_t*)&g_zcl_sceneAttrs[1].customScene    },
    { ZCL_CUSTOM_ATTRID_SCENE_GROUP_ID,     ZCL_UINT16,   RW, (uint8_t*)&g_zcl_sceneAttrs[1].customGroup    },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE2_ATTR_NUM   sizeof(scene2_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t scene3_attrTbl[] = {
    { ZCL_ATTRID_SCENE_SCENE_COUNT,         ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[2].sceneCount     },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,       ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[2].currentScene   },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,       ZCL_UINT16,   R,  (uint8_t*)&g_zcl_sceneAttrs[2].currentGroup   },
    { ZCL_ATTRID_SCENE_SCENE_VALID,         ZCL_BOOLEAN,  R,  (uint8_t*)&g_zcl_sceneAttrs[2].sceneValid     },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,        ZCL_BITMAP8,  R,  (uint8_t*)&g_zcl_sceneAttrs[2].nameSupport    },
    { ZCL_CUSTOM_ATTRID_SCENE_SCENE_ID,     ZCL_UINT8,    RW, (uint8_t*)&g_zcl_sceneAttrs[2].customScene    },
    { ZCL_CUSTOM_ATTRID_SCENE_GROUP_ID,     ZCL_UINT16,   RW, (uint8_t*)&g_zcl_sceneAttrs[2].customGroup    },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE3_ATTR_NUM   sizeof(scene3_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t scene4_attrTbl[] = {
    { ZCL_ATTRID_SCENE_SCENE_COUNT,         ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[3].sceneCount     },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,       ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[3].currentScene   },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,       ZCL_UINT16,   R,  (uint8_t*)&g_zcl_sceneAttrs[3].currentGroup   },
    { ZCL_ATTRID_SCENE_SCENE_VALID,         ZCL_BOOLEAN,  R,  (uint8_t*)&g_zcl_sceneAttrs[3].sceneValid     },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,        ZCL_BITMAP8,  R,  (uint8_t*)&g_zcl_sceneAttrs[3].nameSupport    },
    { ZCL_CUSTOM_ATTRID_SCENE_SCENE_ID,     ZCL_UINT8,    RW, (uint8_t*)&g_zcl_sceneAttrs[3].customScene    },
    { ZCL_CUSTOM_ATTRID_SCENE_GROUP_ID,     ZCL_UINT16,   RW, (uint8_t*)&g_zcl_sceneAttrs[3].customGroup    },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE4_ATTR_NUM   sizeof(scene4_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t scene5_attrTbl[] = {
    { ZCL_ATTRID_SCENE_SCENE_COUNT,         ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[4].sceneCount     },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,       ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[4].currentScene   },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,       ZCL_UINT16,   R,  (uint8_t*)&g_zcl_sceneAttrs[4].currentGroup   },
    { ZCL_ATTRID_SCENE_SCENE_VALID,         ZCL_BOOLEAN,  R,  (uint8_t*)&g_zcl_sceneAttrs[4].sceneValid     },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,        ZCL_BITMAP8,  R,  (uint8_t*)&g_zcl_sceneAttrs[4].nameSupport    },
    { ZCL_CUSTOM_ATTRID_SCENE_SCENE_ID,     ZCL_UINT8,    RW, (uint8_t*)&g_zcl_sceneAttrs[4].customScene    },
    { ZCL_CUSTOM_ATTRID_SCENE_GROUP_ID,     ZCL_UINT16,   RW, (uint8_t*)&g_zcl_sceneAttrs[4].customGroup    },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE5_ATTR_NUM   sizeof(scene5_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t scene6_attrTbl[] = {
    { ZCL_ATTRID_SCENE_SCENE_COUNT,         ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[5].sceneCount     },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,       ZCL_UINT8,    R,  (uint8_t*)&g_zcl_sceneAttrs[5].currentScene   },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,       ZCL_UINT16,   R,  (uint8_t*)&g_zcl_sceneAttrs[5].currentGroup   },
    { ZCL_ATTRID_SCENE_SCENE_VALID,         ZCL_BOOLEAN,  R,  (uint8_t*)&g_zcl_sceneAttrs[5].sceneValid     },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,        ZCL_BITMAP8,  R,  (uint8_t*)&g_zcl_sceneAttrs[5].nameSupport    },
    { ZCL_CUSTOM_ATTRID_SCENE_SCENE_ID,     ZCL_UINT8,    RW, (uint8_t*)&g_zcl_sceneAttrs[5].customScene    },
    { ZCL_CUSTOM_ATTRID_SCENE_GROUP_ID,     ZCL_UINT16,   RW, (uint8_t*)&g_zcl_sceneAttrs[5].customGroup    },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE6_ATTR_NUM   sizeof(scene6_attrTbl) / sizeof(zclAttrInfo_t)

#endif

#ifdef ZCL_ON_OFF_SWITCH_CFG
/* On/Off Config */

zcl_onOffCfgAttr_t g_zcl_onOffCfgAttrs[DEVICE_BUTTON_MAX] = {
    {
        .switchType        = ZCL_SWITCH_TYPE_MULTIFUNCTION,
        .switchActions     = ZCL_SWITCH_ACTION_OFF_ON,
        .custom_swtichType = ZCL_SWITCH_TYPE_TOGGLE,
        .device_model      = DEVICE_MODEL_1,
    },
    {
        .switchType        = ZCL_SWITCH_TYPE_MULTIFUNCTION,
        .switchActions     = ZCL_SWITCH_ACTION_OFF_ON,
        .custom_swtichType = ZCL_SWITCH_TYPE_TOGGLE,
    },
    {
        .switchType        = ZCL_SWITCH_TYPE_MULTIFUNCTION,
        .switchActions     = ZCL_SWITCH_ACTION_OFF_ON,
        .custom_swtichType = ZCL_SWITCH_TYPE_TOGGLE,
    },
    {
        .switchType        = ZCL_SWITCH_TYPE_MULTIFUNCTION,
        .switchActions     = ZCL_SWITCH_ACTION_OFF_ON,
        .custom_swtichType = ZCL_SWITCH_TYPE_TOGGLE,
    },
    {
        .switchType        = ZCL_SWITCH_TYPE_MULTIFUNCTION,
        .switchActions     = ZCL_SWITCH_ACTION_OFF_ON,
        .custom_swtichType = ZCL_SWITCH_TYPE_TOGGLE,
    },
    {
        .switchType        = ZCL_SWITCH_TYPE_MULTIFUNCTION,
        .switchActions     = ZCL_SWITCH_ACTION_OFF_ON,
        .custom_swtichType = ZCL_SWITCH_TYPE_TOGGLE,
    },
};

const zclAttrInfo_t onOffCfg1_attrTbl[] =
{
    { ZCL_ATTRID_SWITCH_TYPE,               ZCL_ENUM8,    R,  (u8*)&g_zcl_onOffCfgAttrs[0].switchType         },
    { ZCL_ATTRID_SWITCH_ACTION,             ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[0].switchActions      },
    { ZCL_CUSTOM_ATTRID_SWITCH_TYPE,        ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[0].custom_swtichType  },
    { ZCL_CUSTOM_ATTRID_DEVICE_MODEL,       ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[0].device_model       },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (u8*)&zcl_attr_global_clusterRevision           },
};

#define ZCL_ON_OFF_CFG1_ATTR_NUM    sizeof(onOffCfg1_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t onOffCfg2_attrTbl[] =
{
    { ZCL_ATTRID_SWITCH_TYPE,               ZCL_ENUM8,    R,  (u8*)&g_zcl_onOffCfgAttrs[1].switchType         },
    { ZCL_ATTRID_SWITCH_ACTION,             ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[1].switchActions      },
    { ZCL_CUSTOM_ATTRID_SWITCH_TYPE,        ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[1].custom_swtichType  },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (u8*)&zcl_attr_global_clusterRevision           },
};

#define ZCL_ON_OFF_CFG2_ATTR_NUM    sizeof(onOffCfg2_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t onOffCfg3_attrTbl[] =
{
    { ZCL_ATTRID_SWITCH_TYPE,               ZCL_ENUM8,    R,  (u8*)&g_zcl_onOffCfgAttrs[2].switchType         },
    { ZCL_ATTRID_SWITCH_ACTION,             ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[2].switchActions      },
    { ZCL_CUSTOM_ATTRID_SWITCH_TYPE,        ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[2].custom_swtichType  },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (u8*)&zcl_attr_global_clusterRevision           },
};

#define ZCL_ON_OFF_CFG3_ATTR_NUM    sizeof(onOffCfg3_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t onOffCfg4_attrTbl[] =
{
    { ZCL_ATTRID_SWITCH_TYPE,               ZCL_ENUM8,    R,  (u8*)&g_zcl_onOffCfgAttrs[3].switchType         },
    { ZCL_ATTRID_SWITCH_ACTION,             ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[3].switchActions      },
    { ZCL_CUSTOM_ATTRID_SWITCH_TYPE,        ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[3].custom_swtichType  },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (u8*)&zcl_attr_global_clusterRevision           },
};

#define ZCL_ON_OFF_CFG4_ATTR_NUM    sizeof(onOffCfg4_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t onOffCfg5_attrTbl[] =
{
    { ZCL_ATTRID_SWITCH_TYPE,               ZCL_ENUM8,    R,  (u8*)&g_zcl_onOffCfgAttrs[4].switchType         },
    { ZCL_ATTRID_SWITCH_ACTION,             ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[4].switchActions      },
    { ZCL_CUSTOM_ATTRID_SWITCH_TYPE,        ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[4].custom_swtichType  },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (u8*)&zcl_attr_global_clusterRevision           },
};

#define ZCL_ON_OFF_CFG5_ATTR_NUM    sizeof(onOffCfg5_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t onOffCfg6_attrTbl[] =
{
    { ZCL_ATTRID_SWITCH_TYPE,               ZCL_ENUM8,    R,  (u8*)&g_zcl_onOffCfgAttrs[5].switchType         },
    { ZCL_ATTRID_SWITCH_ACTION,             ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[5].switchActions      },
    { ZCL_CUSTOM_ATTRID_SWITCH_TYPE,        ZCL_ENUM8,    RW, (u8*)&g_zcl_onOffCfgAttrs[5].custom_swtichType  },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (u8*)&zcl_attr_global_clusterRevision           },
};

#define ZCL_ON_OFF_CFG6_ATTR_NUM    sizeof(onOffCfg6_attrTbl) / sizeof(zclAttrInfo_t)

#endif // ZCL_ON_OFF_SWITCH_CFG

#ifdef ZCL_LEVEL_CTRL
/* Level */
zcl_levelAttr_t g_zcl_levelAttrs[DEVICE_BUTTON_MAX] = {
    {
        .currentLevel = 10,
        .minLevel = ZCL_LEVEL_ATTR_MIN_LEVEL,
        .maxLevel = ZCL_LEVEL_ATTR_MAX_LEVEL,
        .options  = 0,
        .defaultMoveRate = 100,
        .transitionTime = LEVEL_TRANSITION_TIME,
    },
    {
        .currentLevel = 10,
        .minLevel = ZCL_LEVEL_ATTR_MIN_LEVEL,
        .maxLevel = ZCL_LEVEL_ATTR_MAX_LEVEL,
        .options  = 0,
        .defaultMoveRate = 100,
        .transitionTime = LEVEL_TRANSITION_TIME,
    },
    {
        .currentLevel = 10,
        .minLevel = ZCL_LEVEL_ATTR_MIN_LEVEL,
        .maxLevel = ZCL_LEVEL_ATTR_MAX_LEVEL,
        .options  = 0,
        .defaultMoveRate = 100,
        .transitionTime = LEVEL_TRANSITION_TIME,
    },
    {
        .currentLevel = 10,
        .minLevel = ZCL_LEVEL_ATTR_MIN_LEVEL,
        .maxLevel = ZCL_LEVEL_ATTR_MAX_LEVEL,
        .options  = 0,
        .defaultMoveRate = 100,
        .transitionTime = LEVEL_TRANSITION_TIME,
    },
    {
        .currentLevel = 10,
        .minLevel = ZCL_LEVEL_ATTR_MIN_LEVEL,
        .maxLevel = ZCL_LEVEL_ATTR_MAX_LEVEL,
        .options  = 0,
        .defaultMoveRate = 100,
        .transitionTime = LEVEL_TRANSITION_TIME,
    },
    {
        .currentLevel = 10,
        .minLevel = ZCL_LEVEL_ATTR_MIN_LEVEL,
        .maxLevel = ZCL_LEVEL_ATTR_MAX_LEVEL,
        .options  = 0,
        .defaultMoveRate = 100,
        .transitionTime = LEVEL_TRANSITION_TIME,
    },
};

const zclAttrInfo_t level1_attrTbl[] =
{
    { ZCL_ATTRID_LEVEL_CURRENT_LEVEL,       ZCL_UINT8,      RR, (uint8_t*)&g_zcl_levelAttrs[0].currentLevel },
    { ZCL_ATTRID_LEVEL_MIN_LEVEL,           ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[0].minLevel     },
    { ZCL_ATTRID_LEVEL_MAX_LEVEL,           ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[0].maxLevel     },
    { ZCL_ATTRID_LEVEL_OPTIONS,             ZCL_BITMAP8,    RW, (uint8_t*)&g_zcl_levelAttrs[0].options      },
    { ZCL_ATTRID_LEVEL_DEFAULT_MOVE_RATE,   ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[0].defaultMoveRate  },
    { ZCL_ATTRID_LEVEL_ON_OFF_TRANSITION_TIME, ZCL_UINT16,  RW, (uint8_t*)&g_zcl_levelAttrs[0].transitionTime   },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (u8*)&zcl_attr_global_clusterRevision       },
};

#define ZCL_LEVEL1_ATTR_NUM   sizeof(level1_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t level2_attrTbl[] =
{
    { ZCL_ATTRID_LEVEL_CURRENT_LEVEL,       ZCL_UINT8,      RR, (uint8_t*)&g_zcl_levelAttrs[1].currentLevel },
    { ZCL_ATTRID_LEVEL_MIN_LEVEL,           ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[1].minLevel     },
    { ZCL_ATTRID_LEVEL_MAX_LEVEL,           ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[1].maxLevel     },
    { ZCL_ATTRID_LEVEL_OPTIONS,             ZCL_BITMAP8,    RW, (uint8_t*)&g_zcl_levelAttrs[1].options      },
    { ZCL_ATTRID_LEVEL_DEFAULT_MOVE_RATE,   ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[1].defaultMoveRate  },
    { ZCL_ATTRID_LEVEL_ON_OFF_TRANSITION_TIME, ZCL_UINT16,  RW, (uint8_t*)&g_zcl_levelAttrs[1].transitionTime   },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (u8*)&zcl_attr_global_clusterRevision       },
};

#define ZCL_LEVEL2_ATTR_NUM   sizeof(level2_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t level3_attrTbl[] =
{
    { ZCL_ATTRID_LEVEL_CURRENT_LEVEL,       ZCL_UINT8,      RR, (uint8_t*)&g_zcl_levelAttrs[2].currentLevel },
    { ZCL_ATTRID_LEVEL_MIN_LEVEL,           ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[2].minLevel     },
    { ZCL_ATTRID_LEVEL_MAX_LEVEL,           ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[2].maxLevel     },
    { ZCL_ATTRID_LEVEL_OPTIONS,             ZCL_BITMAP8,    RW, (uint8_t*)&g_zcl_levelAttrs[2].options      },
    { ZCL_ATTRID_LEVEL_DEFAULT_MOVE_RATE,   ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[2].defaultMoveRate  },
    { ZCL_ATTRID_LEVEL_ON_OFF_TRANSITION_TIME, ZCL_UINT16,  RW, (uint8_t*)&g_zcl_levelAttrs[2].transitionTime   },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (u8*)&zcl_attr_global_clusterRevision       },
};

#define ZCL_LEVEL3_ATTR_NUM   sizeof(level3_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t level4_attrTbl[] =
{
    { ZCL_ATTRID_LEVEL_CURRENT_LEVEL,       ZCL_UINT8,      RR, (uint8_t*)&g_zcl_levelAttrs[3].currentLevel },
    { ZCL_ATTRID_LEVEL_MIN_LEVEL,           ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[3].minLevel     },
    { ZCL_ATTRID_LEVEL_MAX_LEVEL,           ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[3].maxLevel     },
    { ZCL_ATTRID_LEVEL_OPTIONS,             ZCL_BITMAP8,    RW, (uint8_t*)&g_zcl_levelAttrs[3].options      },
    { ZCL_ATTRID_LEVEL_DEFAULT_MOVE_RATE,   ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[3].defaultMoveRate  },
    { ZCL_ATTRID_LEVEL_ON_OFF_TRANSITION_TIME, ZCL_UINT16,  RW, (uint8_t*)&g_zcl_levelAttrs[3].transitionTime   },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (u8*)&zcl_attr_global_clusterRevision       },
};

#define ZCL_LEVEL4_ATTR_NUM   sizeof(level4_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t level5_attrTbl[] =
{
    { ZCL_ATTRID_LEVEL_CURRENT_LEVEL,       ZCL_UINT8,      RR, (uint8_t*)&g_zcl_levelAttrs[4].currentLevel },
    { ZCL_ATTRID_LEVEL_MIN_LEVEL,           ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[4].minLevel     },
    { ZCL_ATTRID_LEVEL_MAX_LEVEL,           ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[4].maxLevel     },
    { ZCL_ATTRID_LEVEL_OPTIONS,             ZCL_BITMAP8,    RW, (uint8_t*)&g_zcl_levelAttrs[4].options      },
    { ZCL_ATTRID_LEVEL_DEFAULT_MOVE_RATE,   ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[4].defaultMoveRate  },
    { ZCL_ATTRID_LEVEL_ON_OFF_TRANSITION_TIME, ZCL_UINT16,  RW, (uint8_t*)&g_zcl_levelAttrs[4].transitionTime   },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (u8*)&zcl_attr_global_clusterRevision       },
};

#define ZCL_LEVEL5_ATTR_NUM   sizeof(level5_attrTbl) / sizeof(zclAttrInfo_t)

const zclAttrInfo_t level6_attrTbl[] =
{
    { ZCL_ATTRID_LEVEL_CURRENT_LEVEL,       ZCL_UINT8,      RR, (uint8_t*)&g_zcl_levelAttrs[5].currentLevel },
    { ZCL_ATTRID_LEVEL_MIN_LEVEL,           ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[5].minLevel     },
    { ZCL_ATTRID_LEVEL_MAX_LEVEL,           ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[5].maxLevel     },
    { ZCL_ATTRID_LEVEL_OPTIONS,             ZCL_BITMAP8,    RW, (uint8_t*)&g_zcl_levelAttrs[5].options      },
    { ZCL_ATTRID_LEVEL_DEFAULT_MOVE_RATE,   ZCL_UINT8,      RW, (uint8_t*)&g_zcl_levelAttrs[5].defaultMoveRate  },
    { ZCL_ATTRID_LEVEL_ON_OFF_TRANSITION_TIME, ZCL_UINT16,  RW, (uint8_t*)&g_zcl_levelAttrs[5].transitionTime   },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (u8*)&zcl_attr_global_clusterRevision       },
};

#define ZCL_LEVEL6_ATTR_NUM   sizeof(level6_attrTbl) / sizeof(zclAttrInfo_t)

#endif // ZCL_LEVEL_CTRL

/**
 *  @brief Definition for simple switch ZCL specific cluster
 */
const zcl_specClusterInfo_t g_appEp1ClusterList[] = {
    {ZCL_CLUSTER_GEN_BASIC,                 MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM,         basic_attrTbl,      zcl_basic_register,             app_basicCb     },
    {ZCL_CLUSTER_GEN_IDENTIFY,              MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM,      identify_attrTbl,   zcl_identify_register,          app_identifyCb  },
    {ZCL_CLUSTER_GEN_POWER_CFG,             MANUFACTURER_CODE_NONE, ZCL_POWER_CFG_ATTR_NUM,     powerCfg_attrTbl,   zcl_powerCfg_register,          app_powerCfgCb  },
    {ZCL_CLUSTER_GEN_GROUPS,                MANUFACTURER_CODE_NONE, 0,                          NULL,               zcl_group_register,             app_groupCb     },
    {ZCL_CLUSTER_GEN_SCENES,                MANUFACTURER_CODE_NONE, ZCL_SCENE1_ATTR_NUM,        scene1_attrTbl,     zcl_scene_register,             app_sceneCb     },
    {ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,  MANUFACTURER_CODE_NONE, ZCL_ON_OFF_CFG1_ATTR_NUM,   onOffCfg1_attrTbl,  zcl_onOffSwitchCfg_register,    NULL            },
    {ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,MANUFACTURER_CODE_NONE, ZCL_MSINPUT1_ATTR_NUM,      msInput1_attrTbl,   zcl_multistate_input_register,  app_msInputCb   },
    {ZCL_CLUSTER_GEN_LEVEL_CONTROL,         MANUFACTURER_CODE_NONE, ZCL_LEVEL1_ATTR_NUM,        level1_attrTbl,     zcl_level_register,             app_levelCb     },
};

uint8_t APP_EP1_CB_CLUSTER_NUM = (sizeof(g_appEp1ClusterList)/sizeof(g_appEp1ClusterList[0]));

const zcl_specClusterInfo_t g_appEp2ClusterList[] = {
    {ZCL_CLUSTER_GEN_IDENTIFY,              MANUFACTURER_CODE_NONE, ZCL_IDENTIFY2_ATTR_NUM,   identify2_attrTbl, zcl_identify_register,         app_identifyCb  },
    {ZCL_CLUSTER_GEN_GROUPS,                MANUFACTURER_CODE_NONE, 0,                        NULL,              zcl_group_register,            app_groupCb     },
    {ZCL_CLUSTER_GEN_SCENES,                MANUFACTURER_CODE_NONE, ZCL_SCENE2_ATTR_NUM,      scene2_attrTbl,    zcl_scene_register,            app_sceneCb     },
    {ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,  MANUFACTURER_CODE_NONE, ZCL_ON_OFF_CFG2_ATTR_NUM, onOffCfg2_attrTbl, zcl_onOffSwitchCfg_register,   NULL            },
    {ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,MANUFACTURER_CODE_NONE, ZCL_MSINPUT2_ATTR_NUM,    msInput2_attrTbl,  zcl_multistate_input_register, app_msInputCb   },
    {ZCL_CLUSTER_GEN_LEVEL_CONTROL,         MANUFACTURER_CODE_NONE, ZCL_LEVEL2_ATTR_NUM,      level2_attrTbl,    zcl_level_register,            app_levelCb     },
};

uint8_t APP_EP2_CB_CLUSTER_NUM = (sizeof(g_appEp2ClusterList)/sizeof(g_appEp2ClusterList[0]));

const zcl_specClusterInfo_t g_appEp3ClusterList[] = {
    {ZCL_CLUSTER_GEN_IDENTIFY,              MANUFACTURER_CODE_NONE, ZCL_IDENTIFY3_ATTR_NUM,   identify3_attrTbl, zcl_identify_register,         app_identifyCb  },
    {ZCL_CLUSTER_GEN_GROUPS,                MANUFACTURER_CODE_NONE, 0,                        NULL,              zcl_group_register,            app_groupCb     },
    {ZCL_CLUSTER_GEN_SCENES,                MANUFACTURER_CODE_NONE, ZCL_SCENE3_ATTR_NUM,      scene3_attrTbl,    zcl_scene_register,            app_sceneCb     },
    {ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,  MANUFACTURER_CODE_NONE, ZCL_ON_OFF_CFG3_ATTR_NUM, onOffCfg3_attrTbl, zcl_onOffSwitchCfg_register,   NULL            },
    {ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,MANUFACTURER_CODE_NONE, ZCL_MSINPUT3_ATTR_NUM,    msInput3_attrTbl,  zcl_multistate_input_register, app_msInputCb   },
    {ZCL_CLUSTER_GEN_LEVEL_CONTROL,         MANUFACTURER_CODE_NONE, ZCL_LEVEL3_ATTR_NUM,      level3_attrTbl,    zcl_level_register,            app_levelCb     },
};

uint8_t APP_EP3_CB_CLUSTER_NUM = (sizeof(g_appEp3ClusterList)/sizeof(g_appEp3ClusterList[0]));

const zcl_specClusterInfo_t g_appEp4ClusterList[] = {
    {ZCL_CLUSTER_GEN_IDENTIFY,              MANUFACTURER_CODE_NONE, ZCL_IDENTIFY4_ATTR_NUM,   identify4_attrTbl, zcl_identify_register,         app_identifyCb  },
    {ZCL_CLUSTER_GEN_GROUPS,                MANUFACTURER_CODE_NONE, 0,                        NULL,              zcl_group_register,            app_groupCb     },
    {ZCL_CLUSTER_GEN_SCENES,                MANUFACTURER_CODE_NONE, ZCL_SCENE4_ATTR_NUM,      scene4_attrTbl,    zcl_scene_register,            app_sceneCb     },
    {ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,  MANUFACTURER_CODE_NONE, ZCL_ON_OFF_CFG4_ATTR_NUM, onOffCfg4_attrTbl, zcl_onOffSwitchCfg_register,   NULL            },
    {ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,MANUFACTURER_CODE_NONE, ZCL_MSINPUT4_ATTR_NUM,    msInput4_attrTbl,  zcl_multistate_input_register, app_msInputCb   },
    {ZCL_CLUSTER_GEN_LEVEL_CONTROL,         MANUFACTURER_CODE_NONE, ZCL_LEVEL4_ATTR_NUM,      level4_attrTbl,    zcl_level_register,            app_levelCb     },
};

uint8_t APP_EP4_CB_CLUSTER_NUM = (sizeof(g_appEp4ClusterList)/sizeof(g_appEp4ClusterList[0]));

const zcl_specClusterInfo_t g_appEp5ClusterList[] = {
    {ZCL_CLUSTER_GEN_IDENTIFY,              MANUFACTURER_CODE_NONE, ZCL_IDENTIFY5_ATTR_NUM,   identify5_attrTbl, zcl_identify_register,          app_identifyCb  },
    {ZCL_CLUSTER_GEN_GROUPS,                MANUFACTURER_CODE_NONE, 0,                        NULL,              zcl_group_register,             app_groupCb     },
    {ZCL_CLUSTER_GEN_SCENES,                MANUFACTURER_CODE_NONE, ZCL_SCENE5_ATTR_NUM,      scene5_attrTbl,    zcl_scene_register,             app_sceneCb     },
    {ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,  MANUFACTURER_CODE_NONE, ZCL_ON_OFF_CFG5_ATTR_NUM, onOffCfg5_attrTbl, zcl_onOffSwitchCfg_register,    NULL            },
    {ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,MANUFACTURER_CODE_NONE, ZCL_MSINPUT5_ATTR_NUM,    msInput5_attrTbl,  zcl_multistate_input_register,  app_msInputCb   },
    {ZCL_CLUSTER_GEN_LEVEL_CONTROL,         MANUFACTURER_CODE_NONE, ZCL_LEVEL5_ATTR_NUM,      level5_attrTbl,    zcl_level_register,             app_levelCb     },
};

uint8_t APP_EP5_CB_CLUSTER_NUM = (sizeof(g_appEp5ClusterList)/sizeof(g_appEp5ClusterList[0]));

const zcl_specClusterInfo_t g_appEp6ClusterList[] = {
    {ZCL_CLUSTER_GEN_IDENTIFY,              MANUFACTURER_CODE_NONE, ZCL_IDENTIFY6_ATTR_NUM,   identify6_attrTbl, zcl_identify_register,         app_identifyCb  },
    {ZCL_CLUSTER_GEN_GROUPS,                MANUFACTURER_CODE_NONE, 0,                        NULL,              zcl_group_register,            app_groupCb     },
    {ZCL_CLUSTER_GEN_SCENES,                MANUFACTURER_CODE_NONE, ZCL_SCENE6_ATTR_NUM,      scene6_attrTbl,    zcl_scene_register,            app_sceneCb     },
    {ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,  MANUFACTURER_CODE_NONE, ZCL_ON_OFF_CFG6_ATTR_NUM, onOffCfg6_attrTbl, zcl_onOffSwitchCfg_register,   NULL            },
    {ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC,MANUFACTURER_CODE_NONE, ZCL_MSINPUT6_ATTR_NUM,    msInput6_attrTbl,  zcl_multistate_input_register, app_msInputCb   },
    {ZCL_CLUSTER_GEN_LEVEL_CONTROL,         MANUFACTURER_CODE_NONE, ZCL_LEVEL6_ATTR_NUM,      level6_attrTbl,    zcl_level_register,            app_levelCb     },
};

uint8_t APP_EP6_CB_CLUSTER_NUM = (sizeof(g_appEp6ClusterList)/sizeof(g_appEp6ClusterList[0]));

