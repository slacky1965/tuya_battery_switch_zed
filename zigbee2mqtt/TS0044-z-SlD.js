import * as m from 'zigbee-herdsman-converters/lib/modernExtend';
import * as exposes_1 from 'zigbee-herdsman-converters/lib/exposes';
import * as reporting from 'zigbee-herdsman-converters/lib/reporting';
import * as utils_1 from 'zigbee-herdsman-converters/lib/utils';
import * as logger from 'zigbee-herdsman-converters/lib/logger';

const batteryReporting = {min: 3600, max: 14400, change: 0};

const attrSwitchType = 0xf000;
const attrDeviceModelNumber = 0xf002;
const attrSceneId = 0xf000;
const attrGroupId = 0xf001;

function localActionExtend(args = {}) {
    const { localAction = ["hold", "single", "double", "triple", "quadruple", "quintuple", "release"], 
            bind = true,
            reporting = true, 
            reportingConfig = {"min": 10, "max": 0, change: 1},
            endpointNames = undefined } = args;
    let actions = localAction;
    if (endpointNames) {
        actions = localAction.flatMap((c) => endpointNames.map((e) => `${c}_${e}`));
    }
    const exposes = [exposes_1.presets.enum("action", exposes_1.access.STATE, actions)];
    const attribute = "presentValue";

    
    const actionPayloadLookup = {
      0: "hold",
      1: "single",
      2: "double",
      3: "triple",
      4: "quadruple",
      5: "quintuple",
      255: "release",
    };

    const fromZigbee = [
        {
            cluster: "genMultistateInput",
            type: ["attributeReport", "readResponse"],
            convert: (model, msg, publish, options, meta) => {
                if ((0, utils_1.hasAlreadyProcessedMessage)(msg, model))
                    return;
                const data = msg.data;
                //logger.logger.info('msg.data: ' + data[attribute]);
                if (data[attribute] == 300)
                    return "N/A";
                const payload = { action: (0, utils_1.postfixWithEndpointName)(actionPayloadLookup[data[attribute]], msg, model, meta) };
                return payload;
            },
        },
    ];
    const result = { exposes, fromZigbee, isModernExtend: true };
    if (reporting)
        result.configure = [m.setupConfigureForBinding("genMultistateInput", "input", endpointNames),
                            m.setupConfigureForReporting("genMultistateInput", attribute, {config: reportingConfig, access: exposes_1.access.GET, endpointNames})];
    else if (bind) result.configure = [m.setupConfigureForBinding("genMultistateInput", "input", endpointNames)];

    return result;

}

export default {
        zigbeeModel: ["TS0044-z-SlD"],
        model: "TS0044-z-SlD",
        vendor: "Slacky-DIY",
        description: "Tuya wireless switch with 4 buttons with custom firmware",
        extend: [
            m.deviceAddCustomCluster("genOnOffSwitchCfg", {
                ID: 0x0007,
                attributes: {
                    customSwitchType: {
                        ID: attrSwitchType,
                        type: 0x30,
                        write: true,
                        max: 0xff,
                    },
                    customDeviceModelNumber: {
                        ID: attrDeviceModelNumber,
                        type: 0x30,
                        write: true,
                        max: 0xff,
                    },
                },
                commands: {},
                commandsResponse: {},
            }),
            m.deviceAddCustomCluster("genScenes", {
                ID: 0x0005,
                attributes: {
                    customSceneId: {
                        ID: attrSceneId,
                        type: 0x20,
                        write: true,
                        max: 0xff,
                    },
                    customGroupId: {
                        ID: attrGroupId,
                        type: 0x21,
                        write: true,
                        max: 0xffff,
                    },
                },
                commands: {},
                commandsResponse: {},
            }),
            m.deviceEndpoints({endpoints: {"1": 1, "2": 2, "3": 3, "4": 4}}),
            m.battery({
                percentageReportingConfig: batteryReporting,
            }),
            m.commandsOnOff({endpointNames: ["1", "2", "3", "4"]}),
            localActionExtend({
                endpointNames: ["1", "2", "3", "4"],
                reporting: false,
            }),
            m.commandsLevelCtrl({endpointNames: ["1", "2", "3", "4"]}),
            m.enumLookup({
                name: "switch_actions",
                endpointName: "1",
                lookup: {off: 0, on: 1, toggle: 2},
                cluster: "genOnOffSwitchCfg",
                attribute: "switchActions",
                description: "Actions switch",
            }),
            m.enumLookup({
                name: "switch_actions",
                endpointName: "2",
                lookup: {off: 0, on: 1, toggle: 2},
                cluster: "genOnOffSwitchCfg",
                attribute: "switchActions",
                description: "Actions switch",
            }),
            m.enumLookup({
                name: "switch_actions",
                endpointName: "3",
                lookup: {off: 0, on: 1, toggle: 2},
                cluster: "genOnOffSwitchCfg",
                attribute: "switchActions",
                description: "Actions switch",
            }),
            m.enumLookup({
                name: "switch_actions",
                endpointName: "4",
                lookup: {off: 0, on: 1, toggle: 2},
                cluster: "genOnOffSwitchCfg",
                attribute: "switchActions",
                description: "Actions switch",
            }),
            m.enumLookup({
                name: "switch_type",
                endpointName: "1",
                lookup: {
                    toggle: 0,
                    momentary: 1,
                    multifunction: 2,
                    brightness_level_up: 3,
                    brightness_level_down: 4,
                    scene: 5,
                },
                cluster: "genOnOffSwitchCfg",
                attribute: "customSwitchType",
                description: "Switch type",
            }),
            m.enumLookup({
                name: "switch_type",
                endpointName: "2",
                lookup: {
                    toggle: 0,
                    momentary: 1,
                    multifunction: 2,
                    brightness_level_up: 3,
                    brightness_level_down: 4,
                    scene: 5,
                },
                cluster: "genOnOffSwitchCfg",
                attribute: "customSwitchType",
                description: "Switch type",
            }),
            m.enumLookup({
                name: "switch_type",
                endpointName: "3",
                lookup: {
                    toggle: 0,
                    momentary: 1,
                    multifunction: 2,
                    brightness_level_up: 3,
                    brightness_level_down: 4,
                    scene: 5,
                },
                cluster: "genOnOffSwitchCfg",
                attribute: "customSwitchType",
                description: "Switch type",
            }),
            m.enumLookup({
                name: "switch_type",
                endpointName: "4",
                lookup: {
                    toggle: 0,
                    momentary: 1,
                    multifunction: 2,
                    brightness_level_up: 3,
                    brightness_level_down: 4,
                    scene: 5,
                },
                cluster: "genOnOffSwitchCfg",
                attribute: "customSwitchType",
                description: "Switch type",
            }),
            m.commandsScenes({endpointNames: ["1", "2", "3", "4"]}),
            m.numeric({
                name: "scene_id",
                endpointNames: ["1", "2", "3", "4"],
                access: "ALL",
                cluster: "genScenes",
                attribute: "customSceneId",
                description: "Scene ID",
            }),
            m.numeric({
                name: "group_id",
                endpointNames: ["1", "2", "3", "4"],
                access: "ALL",
                cluster: "genScenes",
                attribute: "customGroupId",
                description: "Group ID for scenes",
            }),
        ],
        meta: {},
        ota: true,
};
