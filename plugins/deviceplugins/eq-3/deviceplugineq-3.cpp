/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  This file is part of guh.                                              *
 *                                                                         *
 *  Guh is free software: you can redistribute it and/or modify            *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, version 2 of the License.                *
 *                                                                         *
 *  Guh is distributed in the hope that it will be useful,                 *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with guh. If not, see <http://www.gnu.org/licenses/>.            *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "deviceplugineq-3.h"

#include "plugin/device.h"
#include "devicemanager.h"
#include "types/param.h"

#include <QDebug>

VendorId eq3VendorId = VendorId("2cac0645-855e-44fa-837e-1cab0ae4304c");

PluginId eq3PluginUuid = PluginId("f324c43c-9680-48d8-852a-93b2227139b9");

DeviceClassId cubeDeviceClassId = DeviceClassId("1e892268-8bd7-442c-a001-bd4e2e6b2949");
StateTypeId connectionStateTypeId = StateTypeId("d0a9a369-cf8c-47c4-a12e-f2d076bf12fd");
StateTypeId portalEnabeldStateTypeId = StateTypeId("2c2367da-c229-40ed-9d47-a6e73cd6dc3b");

DeviceClassId wallThermostateDeviceClassId = DeviceClassId("ffbfec5d-06e8-4082-b62b-92cc5c3e8c4e");

StateTypeId confortTempStateTypeId = StateTypeId("850380ee-a787-43e7-adb8-768a21a6e64d");
StateTypeId ecoTempStateTypeId = StateTypeId("24dfd20d-bc8d-48e4-8162-b20ae0465c41");
StateTypeId maxSetpointTempStateTypeId = StateTypeId("a8536ddf-a6e4-41c2-89c1-e7102608f5f6");
StateTypeId minSetpointTempStateTypeId = StateTypeId("ceb0ad05-37ad-4b79-a4d9-540c34a7e3e4");
StateTypeId errorOccuredStateTypeId = StateTypeId("9880247b-cf9a-453c-b0c3-d910eba8a253");
StateTypeId initializedStateTypeId = StateTypeId("a9e29f03-063e-4686-8aac-2f6d8f8a4937");
StateTypeId batteryLowStateTypeId = StateTypeId("53b89f32-8894-4290-92a0-6a470c6b69ab");
StateTypeId linkStatusOKStateTypeId = StateTypeId("aff38be8-7ea6-4fd8-b0fa-e987ab05c719");
StateTypeId panelLockedStateTypeId = StateTypeId("979df197-09a1-46f9-9217-9d323b1062bd");
StateTypeId gatewayKnownStateTypeId = StateTypeId("1d6bd962-5c31-47ad-80a4-dda87bff98f5");
StateTypeId dtsActiveStateTypeId = StateTypeId("1b402ba6-a8ae-45b1-8acf-2b0a89f71889");
StateTypeId deviceModeStateTypeId = StateTypeId("639360f0-bb65-43e6-b227-50ae0ac39d6c");
StateTypeId deviceModeStringStateTypeId = StateTypeId("ff5194e3-5641-4ac2-92c7-48c431b4a2eb");
StateTypeId setpointTempStateTypeId = StateTypeId("579aa8c6-8814-491b-9e7c-b98108c323d1");
StateTypeId currentTemperatureStateTypeId = StateTypeId("852e7708-db1d-42d1-96e4-19c13598262c");

ActionTypeId setSetpointTemperatureActionTypeId = ActionTypeId("9c1968ba-39f9-493d-9fe2-848fa86bd2f0");
ActionTypeId setAutoModeActionTypeId = ActionTypeId("162b4b3d-9923-4f2c-a755-b50c8a06a6f0");
ActionTypeId setManuelModeActionTypeId = ActionTypeId("8e604437-9f5b-4c17-b5b0-e2db6007af5b");
ActionTypeId setEcoModeActionTypeId = ActionTypeId("27a981e8-ec23-4ba8-921e-33b911a7dd89");
ActionTypeId displayCurrentTempActionTypeId = ActionTypeId("184fb112-7a03-4560-8634-0257c969c26e");

DeviceClassId radiatorThermostateDeviceClassId = DeviceClassId("f80d9481-4827-45ee-a013-b97b22412d92");
StateTypeId valvePositionStateTypeId = StateTypeId("72956000-0203-4c32-a6b6-3bb7e46c03ca");

StateTypeId offsetTempStateTypeId = StateTypeId("576da571-9a65-478f-96bf-19256c8b9ece");
StateTypeId windowOpenDurationStateTypeId = StateTypeId("81c6c74a-b0cd-4daa-9eb9-f1cd68f328af");
StateTypeId boostValueValueStateTypeId = StateTypeId("7c41fa64-b1a1-48d2-9d03-67aa16cd83ad");
StateTypeId boostDurationStateTypeId = StateTypeId("e75c1398-9ad7-466c-b3b9-b03bbb686a30");
StateTypeId discalcWeekDayStateTypeId = StateTypeId("bd6f5947-d4b4-444b-81c8-77eec46957e4");
StateTypeId discalcTimeStateTypeId = StateTypeId("e78235ee-affc-41e3-a463-9f0512b4a6c3");
StateTypeId valveMaximumSettingsStateTypeId = StateTypeId("e367fa3a-b30f-49bd-af3f-cff92360ad32");
StateTypeId valveOffsetStateTypeId = StateTypeId("ffaff87b-b741-4db8-9875-3380af4f1885");


DevicePluginEQ3::DevicePluginEQ3()
{
    m_cubeDiscovery = new MaxCubeDiscovery(this);

    connect(m_cubeDiscovery,SIGNAL(cubesDetected(QList<MaxCube*>)),this,SLOT(discoveryDone(QList<MaxCube*>)));
}

QList<Vendor> DevicePluginEQ3::supportedVendors() const
{
    QList<Vendor> ret;
    Vendor eq3(eq3VendorId, "eQ-3");
    ret.append(eq3);
    return ret;
}

QList<DeviceClass> DevicePluginEQ3::supportedDevices() const
{
    QList<DeviceClass> ret;

    // ===========================================
    // Cube
    DeviceClass cubeDeviceClass(pluginId(),eq3VendorId,cubeDeviceClassId);
    cubeDeviceClass.setName("Max! Cube LAN Gateway");
    cubeDeviceClass.setCreateMethod(DeviceClass::CreateMethodDiscovery);
    cubeDeviceClass.setSetupMethod(DeviceClass::SetupMethodJustAdd);

    // Params
    QList<ParamType> params;
    ParamType hostParam("host address", QVariant::String);
    params.append(hostParam);

    ParamType portParam("port", QVariant::Int);
    params.append(portParam);

    ParamType serialNumberParam("serial number", QVariant::String);
    params.append(serialNumberParam);

    ParamType firmwareParam("firmware version", QVariant::Int);
    params.append(firmwareParam);

    cubeDeviceClass.setParamTypes(params);

    // States
    QList<StateType> states;
    StateType connectedState(connectionStateTypeId);
    connectedState.setName("connected");
    connectedState.setType(QVariant::Bool);
    connectedState.setDefaultValue(false);
    states.append(connectedState);

    StateType portalEnabeld(portalEnabeldStateTypeId);
    portalEnabeld.setName("portal enabled");
    portalEnabeld.setType(QVariant::Bool);
    portalEnabeld.setDefaultValue(false);
    states.append(portalEnabeld);
    cubeDeviceClass.setStateTypes(states);

    // ===========================================
    // Wall thermostate
    DeviceClass wallThermostateDeviceClass(pluginId(), eq3VendorId, wallThermostateDeviceClassId);
    wallThermostateDeviceClass.setName("Max! Wall Thermostat");
    wallThermostateDeviceClass.setCreateMethod(DeviceClass::CreateMethodAuto);

    // Params
    QList<ParamType> paramsWallThermostate;

    paramsWallThermostate.append(serialNumberParam);

    ParamType deviceName("name",QVariant::String);
    paramsWallThermostate.append(deviceName);

    ParamType parentCube("parent cube",QVariant::String);
    paramsWallThermostate.append(parentCube);

    ParamType rfAddress("rf address",QVariant::String);
    paramsWallThermostate.append(rfAddress);

    ParamType roomNumber("room id",QVariant::Int);
    paramsWallThermostate.append(roomNumber);

    ParamType roomName("room name",QVariant::String);
    paramsWallThermostate.append(roomName);

    wallThermostateDeviceClass.setParamTypes(paramsWallThermostate);


    // Actions
    QList<ActionType> actions;

    ActionType setSetpointTemp(setSetpointTemperatureActionTypeId);
    setSetpointTemp.setName("set setpoint temperature [Celsius]");
    QList<ParamType> actionParamsSetSetpointTemp;
    ParamType actionParamSetpointTemperature("setpoint temperature", QVariant::Double);
    actionParamsSetSetpointTemp.append(actionParamSetpointTemperature);
    setSetpointTemp.setParameters(actionParamsSetSetpointTemp);
    actions.append(setSetpointTemp);

    ActionType setAutoMode(setAutoModeActionTypeId);
    setAutoMode.setName("set Auto mode");
    actions.append(setAutoMode);

    ActionType setManuelMode(setManuelModeActionTypeId);
    setManuelMode.setName("set Manuel mode");
    actions.append(setAutoMode);

    ActionType setEcoMode(setEcoModeActionTypeId);
    setEcoMode.setName("set Eco mode");
    actions.append(setEcoMode);

    ActionType displayCurrentTemp(displayCurrentTempActionTypeId);
    displayCurrentTemp.setName("display current temperature");
    QList<ParamType> actionParamsDisplayCurrentTemp;
    ParamType actionParamDisplayCurrentTemp("display", QVariant::Bool);
    actionParamsDisplayCurrentTemp.append(actionParamDisplayCurrentTemp);
    displayCurrentTemp.setParameters(actionParamsDisplayCurrentTemp);
    actions.append(displayCurrentTemp);

    wallThermostateDeviceClass.setActions(actions);

    // States
    QList<StateType> statesWallThermostat;

    StateType confortTemp(confortTempStateTypeId);
    confortTemp.setName("confort temperature [Celsius]");
    confortTemp.setType(QVariant::Double);
    confortTemp.setDefaultValue(0.0);
    statesWallThermostat.append(confortTemp);

    StateType ecoTemp(ecoTempStateTypeId);
    ecoTemp.setName("eco temperature [Celsius]");
    ecoTemp.setType(QVariant::Double);
    ecoTemp.setDefaultValue(0.0);
    statesWallThermostat.append(ecoTemp);

    StateType minSetpointTemp(minSetpointTempStateTypeId);
    minSetpointTemp.setName("min setpoint temperature [Celsius]");
    minSetpointTemp.setType(QVariant::Double);
    minSetpointTemp.setDefaultValue(0.0);
    statesWallThermostat.append(minSetpointTemp);

    StateType maxSetpointTemp(maxSetpointTempStateTypeId);
    maxSetpointTemp.setName("max setpoint temperature [Celsius]");
    maxSetpointTemp.setType(QVariant::Double);
    maxSetpointTemp.setDefaultValue(0.0);
    statesWallThermostat.append(maxSetpointTemp);

    StateType errorOccured(errorOccuredStateTypeId);
    errorOccured.setName("error occured");
    errorOccured.setType(QVariant::Bool);
    errorOccured.setDefaultValue(false);
    statesWallThermostat.append(errorOccured);

    StateType initialized(initializedStateTypeId);
    initialized.setName("initialized");
    initialized.setType(QVariant::Bool);
    initialized.setDefaultValue(false);
    statesWallThermostat.append(initialized);

    StateType batteryLow(batteryLowStateTypeId);
    batteryLow.setName("battery low");
    batteryLow.setType(QVariant::Bool);
    batteryLow.setDefaultValue(false);
    statesWallThermostat.append(batteryLow);

    StateType linkStatusOK(linkStatusOKStateTypeId);
    linkStatusOK.setName("link status to cube");
    linkStatusOK.setType(QVariant::Bool);
    linkStatusOK.setDefaultValue(false);
    statesWallThermostat.append(linkStatusOK);

    StateType panelLocked(panelLockedStateTypeId);
    panelLocked.setName("pannel locked");
    panelLocked.setType(QVariant::Bool);
    panelLocked.setDefaultValue(false);
    statesWallThermostat.append(panelLocked);

    StateType gatewayKnown(gatewayKnownStateTypeId);
    gatewayKnown.setName("gateway knows");
    gatewayKnown.setType(QVariant::Bool);
    gatewayKnown.setDefaultValue(false);
    statesWallThermostat.append(gatewayKnown);

    StateType dtsActive(dtsActiveStateTypeId);
    dtsActive.setName("DTS active");
    dtsActive.setType(QVariant::Bool);
    dtsActive.setDefaultValue(false);
    statesWallThermostat.append(dtsActive);

    StateType deviceMode(deviceModeStateTypeId);
    deviceMode.setName("device mode");
    deviceMode.setType(QVariant::Int);
    deviceMode.setDefaultValue(-1);
    statesWallThermostat.append(deviceMode);

    StateType deviceModeString(deviceModeStringStateTypeId);
    deviceModeString.setName("device mode string");
    deviceModeString.setType(QVariant::String);
    deviceModeString.setDefaultValue("");
    statesWallThermostat.append(deviceModeString);

    StateType setpointTemp(setpointTempStateTypeId);
    setpointTemp.setName("setpoint temperature [Celsius]");
    setpointTemp.setType(QVariant::Double);
    setpointTemp.setDefaultValue("0.0");
    statesWallThermostat.append(setpointTemp);

    StateType currentTemp(currentTemperatureStateTypeId);
    currentTemp.setName("current temperature [Celsius]");
    currentTemp.setType(QVariant::Double);
    currentTemp.setDefaultValue(0.0);
    statesWallThermostat.append(currentTemp);

    wallThermostateDeviceClass.setStateTypes(statesWallThermostat);


    // ===========================================
    // Radiator thermostat
    DeviceClass radiatorThermostateDeviceClass(pluginId(), eq3VendorId, radiatorThermostateDeviceClassId);
    radiatorThermostateDeviceClass.setName("Max! Radiator Thermostat");
    radiatorThermostateDeviceClass.setCreateMethod(DeviceClass::CreateMethodAuto);

    // Params
    QList<ParamType> paramsRadiatorThermostate;

    paramsRadiatorThermostate.append(serialNumberParam);
    paramsRadiatorThermostate.append(deviceName);
    paramsRadiatorThermostate.append(parentCube);
    paramsRadiatorThermostate.append(rfAddress);
    paramsRadiatorThermostate.append(roomNumber);
    paramsWallThermostate.append(roomName);

    radiatorThermostateDeviceClass.setParamTypes(paramsRadiatorThermostate);


    // Actions
    QList<ActionType> actionsRadiator;

    actionsRadiator.append(setSetpointTemp);
    actionsRadiator.append(setAutoMode);
    actionsRadiator.append(setManuelMode);
    actionsRadiator.append(setEcoMode);

    radiatorThermostateDeviceClass.setActions(actionsRadiator);

    // States
    QList<StateType> statesRadiatorThermostat;
    statesRadiatorThermostat.append(confortTemp);
    statesRadiatorThermostat.append(ecoTemp);
    statesRadiatorThermostat.append(minSetpointTemp);
    statesRadiatorThermostat.append(maxSetpointTemp);
    statesRadiatorThermostat.append(errorOccured);
    statesRadiatorThermostat.append(initialized);
    statesRadiatorThermostat.append(batteryLow);
    statesRadiatorThermostat.append(linkStatusOK);
    statesRadiatorThermostat.append(gatewayKnown);
    statesRadiatorThermostat.append(dtsActive);
    statesRadiatorThermostat.append(deviceMode);
    statesRadiatorThermostat.append(deviceModeString);
    statesRadiatorThermostat.append(setpointTemp);

    StateType offsetTemp(offsetTempStateTypeId);
    offsetTemp.setName("offset temperature [Celsius]");
    offsetTemp.setType(QVariant::Double);
    offsetTemp.setDefaultValue(0.0);
    statesRadiatorThermostat.append(offsetTemp);

    StateType windowOpenDuration(windowOpenDurationStateTypeId);
    windowOpenDuration.setName("window open duration [Minutes]");
    windowOpenDuration.setType(QVariant::Int);
    windowOpenDuration.setDefaultValue(0);
    statesRadiatorThermostat.append(windowOpenDuration);

    StateType boostValveValue(boostValueValueStateTypeId);
    boostValveValue.setName("boost valve value [%]");
    boostValveValue.setType(QVariant::Int);
    boostValveValue.setDefaultValue(0);
    statesRadiatorThermostat.append(boostValveValue);

    StateType boostDuration(boostDurationStateTypeId);
    boostDuration.setName("boost duration [Minutes]");
    boostDuration.setType(QVariant::Int);
    boostDuration.setDefaultValue(0);
    statesRadiatorThermostat.append(boostDuration);

    StateType discalcWeekDay(discalcWeekDayStateTypeId);
    discalcWeekDay.setName("discalc day");
    discalcWeekDay.setType(QVariant::String);
    discalcWeekDay.setDefaultValue("");
    statesRadiatorThermostat.append(discalcWeekDay);

    StateType discalcTime(discalcTimeStateTypeId);
    discalcTime.setName("discalc time");
    discalcTime.setType(QVariant::String);
    discalcTime.setDefaultValue("");
    statesRadiatorThermostat.append(discalcTime);

    StateType maxValveSettings(valveMaximumSettingsStateTypeId);
    maxValveSettings.setName("valve maximum settings");
    maxValveSettings.setType(QVariant::Double);
    maxValveSettings.setDefaultValue(0.0);
    statesRadiatorThermostat.append(maxValveSettings);

    StateType valveOffset(valveOffsetStateTypeId);
    valveOffset.setName("valve offset [%]");
    valveOffset.setType(QVariant::Int);
    valveOffset.setDefaultValue(0);
    statesRadiatorThermostat.append(valveOffset);

    radiatorThermostateDeviceClass.setStateTypes(statesRadiatorThermostat);



    ret.append(cubeDeviceClass);
    ret.append(wallThermostateDeviceClass);
    ret.append(radiatorThermostateDeviceClass);

    return ret;
}

DeviceManager::HardwareResources DevicePluginEQ3::requiredHardware() const
{
    return DeviceManager::HardwareResourceTimer;
}

QString DevicePluginEQ3::pluginName() const
{
    return "eQ-3";
}

PluginId DevicePluginEQ3::pluginId() const
{
    return eq3PluginUuid;
}

QList<ParamType> DevicePluginEQ3::configurationDescription() const
{
    QList<ParamType> params;
    return params;
}

QPair<DeviceManager::DeviceError, QString> DevicePluginEQ3::discoverDevices(const DeviceClassId &deviceClassId, const ParamList &params)
{
    if(deviceClassId == cubeDeviceClassId){
        m_cubeDiscovery->detectCubes();
        return report(DeviceManager::DeviceErrorAsync);
    }
    return report(DeviceManager::DeviceErrorDeviceClassNotFound);
}

void DevicePluginEQ3::startMonitoringAutoDevices()
{

}

QPair<DeviceManager::DeviceSetupStatus, QString> DevicePluginEQ3::setupDevice(Device *device)
{
    qDebug() << "setupDevice" << device->params();

    if(device->deviceClassId() == cubeDeviceClassId){
        foreach (MaxCube *cube, m_cubes.keys()) {
            if(cube->serialNumber() == device->paramValue("serial number").toString()){
                qDebug() << cube->serialNumber() << " allready exists...";
                return reportDeviceSetup(DeviceManager::DeviceSetupStatusFailure,QString("Cube allready in added"));
            }
        }

        MaxCube *cube = new MaxCube(this,device->paramValue("serial number").toString(),QHostAddress(device->paramValue("host address").toString()),device->paramValue("port").toInt());
        m_cubes.insert(cube,device);

        connect(cube,SIGNAL(cubeConnectionStatusChanged(bool)),this,SLOT(cubeConnectionStatusChanged(bool)));
        connect(cube,SIGNAL(commandActionFinished(bool,ActionId)),this,SLOT(commandActionFinished(bool,ActionId)));
        connect(cube,SIGNAL(cubeConfigReady()),this,SLOT(updateCubeConfig()));
        connect(cube,SIGNAL(wallThermostatFound()),this,SLOT(wallThermostatFound()));
        connect(cube,SIGNAL(wallThermostatDataUpdated()),this,SLOT(wallThermostatDataUpdated()));
        connect(cube,SIGNAL(radiatorThermostatFound()),this,SLOT(radiatorThermostatFound()));
        connect(cube,SIGNAL(radiatorThermostatDataUpdated()),this,SLOT(radiatorThermostatDataUpdated()));

        cube->connectToCube();

        return reportDeviceSetup(DeviceManager::DeviceSetupStatusAsync);
    }
    if(device->deviceClassId() == wallThermostateDeviceClassId){
        device->setName("Max! Wall Thermostat (" + device->paramValue("serial number").toString() + ")");
    }

    return reportDeviceSetup();
}

void DevicePluginEQ3::deviceRemoved(Device *device)
{
    if (!m_cubes.values().contains(device)) {
        return;
    }

    MaxCube *cube = m_cubes.key(device);
    cube->disconnectFromCube();
    qDebug() << "remove cube " << cube->serialNumber();
    m_cubes.remove(cube);
    cube->deleteLater();
}

void DevicePluginEQ3::guhTimer()
{
    foreach (MaxCube *cube, m_cubes.keys()){
        if(cube->isConnected() && cube->isInitialized()){
            cube->refresh();
        }
    }
}

QPair<DeviceManager::DeviceError, QString> DevicePluginEQ3::executeAction(Device *device, const Action &action)
{    
    if(device->deviceClassId() == wallThermostateDeviceClassId || device->deviceClassId() == radiatorThermostateDeviceClassId){
        foreach (MaxCube *cube, m_cubes.keys()){
            if(cube->serialNumber() == device->paramValue("parent cube").toString()){
                if (action.actionTypeId() == setSetpointTemperatureActionTypeId){
                    cube->setDeviceSetpointTemp(device->paramValue("rf address").toByteArray(), device->paramValue("room id").toInt(), action.param("setpoint temperature").value().toDouble(), action.id());
                    return report(DeviceManager::DeviceErrorAsync,QString());
                }
                if (action.actionTypeId() == setAutoModeActionTypeId){
                    cube->setDeviceAutoMode(device->paramValue("rf address").toByteArray(), device->paramValue("room id").toInt(), action.id());
                    return report(DeviceManager::DeviceErrorAsync,QString());
                }
                if (action.actionTypeId() == setManuelModeActionTypeId){
                    cube->setDeviceManuelMode(device->paramValue("rf address").toByteArray(), device->paramValue("room id").toInt(), action.id());
                    return report(DeviceManager::DeviceErrorAsync,QString());
                }
                if (action.actionTypeId() == setEcoModeActionTypeId){
                    cube->setDeviceEcoMode(device->paramValue("rf address").toByteArray(), device->paramValue("room id").toInt(), action.id());
                    return report(DeviceManager::DeviceErrorAsync,QString());
                }
                if (action.actionTypeId() == displayCurrentTempActionTypeId){
                    cube->displayCurrentTemperature(device->paramValue("rf address").toByteArray(), device->paramValue("room id").toInt(), action.param("display").value().toBool(), action.id());
                    return report(DeviceManager::DeviceErrorAsync,QString());
                }
            }
        }
    }

    return report(DeviceManager::DeviceErrorActionTypeNotFound,QString());
}

void DevicePluginEQ3::cubeConnectionStatusChanged(const bool &connected)
{
    if(connected){
        MaxCube *cube = static_cast<MaxCube*>(sender());
        Device *device;
        if (m_cubes.contains(cube)) {
            device = m_cubes.value(cube);
            device->setName("Max! Cube " + cube->serialNumber());
            device->setStateValue(connectionStateTypeId,true);
            emit deviceSetupFinished(device, DeviceManager::DeviceSetupStatusSuccess, QString());
        }
    }else{
        MaxCube *cube = static_cast<MaxCube*>(sender());
        Device *device;
        if (m_cubes.contains(cube)){
            device = m_cubes.value(cube);
            device->setStateValue(connectionStateTypeId,false);
            emit deviceSetupFinished(device, DeviceManager::DeviceSetupStatusFailure, QString("Could not connect to cube."));
        }
    }
}

void DevicePluginEQ3::discoveryDone(const QList<MaxCube *> &cubeList)
{
    QList<DeviceDescriptor> retList;
    foreach (MaxCube *cube, cubeList) {
        DeviceDescriptor descriptor(cubeDeviceClassId, "Max! Cube LAN Gateway",cube->serialNumber());
        ParamList params;
        Param hostParam("host address", cube->hostAddress().toString());
        params.append(hostParam);
        Param portParam("port", cube->port());
        params.append(portParam);
        Param firmwareParam("firmware version", cube->firmware());
        params.append(firmwareParam);
        Param serialNumberParam("serial number", cube->serialNumber());
        params.append(serialNumberParam);

        descriptor.setParams(params);
        retList.append(descriptor);
    }
    emit devicesDiscovered(cubeDeviceClassId,retList);
}

void DevicePluginEQ3::commandActionFinished(const bool &succeeded, const ActionId &actionId)
{
    if(succeeded){
        emit actionExecutionFinished(actionId, DeviceManager::DeviceErrorNoError,QString());
    }else{
        emit actionExecutionFinished(actionId, DeviceManager::DeviceErrorSetupFailed,QString("Could not execute action on cube, unknown error"));
    }
}

void DevicePluginEQ3::wallThermostatFound()
{
    MaxCube *cube = static_cast<MaxCube*>(sender());

    QList<DeviceDescriptor> descriptorList;

    foreach (WallThermostat *wallThermostat, cube->wallThermostatList()) {
        bool allreadyAdded = false;
        foreach (Device *device, deviceManager()->findConfiguredDevices(wallThermostateDeviceClassId)){
            if(wallThermostat->serialNumber() == device->paramValue("serial number").toString()){
                allreadyAdded = true;
                break;
            }
        }
        if(!allreadyAdded){
            DeviceDescriptor descriptor(wallThermostateDeviceClassId, wallThermostat->serialNumber());
            ParamList params;
            params.append(Param("name", wallThermostat->deviceName()));
            params.append(Param("parent cube", cube->serialNumber()));
            params.append(Param("serial number", wallThermostat->serialNumber()));
            params.append(Param("rf address", wallThermostat->rfAddress()));
            params.append(Param("room id", wallThermostat->roomId()));
            params.append(Param("room name", wallThermostat->roomName()));
            descriptor.setParams(params);
            descriptorList.append(descriptor);
        }
    }

    if(!descriptorList.isEmpty()){
        metaObject()->invokeMethod(this, "autoDevicesAppeared", Qt::QueuedConnection, Q_ARG(DeviceClassId, wallThermostateDeviceClassId), Q_ARG(QList<DeviceDescriptor>, descriptorList));
    }

}

void DevicePluginEQ3::radiatorThermostatFound()
{
    MaxCube *cube = static_cast<MaxCube*>(sender());

    QList<DeviceDescriptor> descriptorList;

    foreach (RadiatorThermostat *radiatorThermostat, cube->radiatorThermostatList()) {
        bool allreadyAdded = false;
        foreach (Device *device, deviceManager()->findConfiguredDevices(radiatorThermostateDeviceClassId)){
            if(radiatorThermostat->serialNumber() == device->paramValue("serial number").toString()){
                allreadyAdded = true;
                break;
            }
        }
        if(!allreadyAdded){
            DeviceDescriptor descriptor(radiatorThermostateDeviceClassId, radiatorThermostat->serialNumber());
            ParamList params;
            params.append(Param("name", radiatorThermostat->deviceName()));
            params.append(Param("parent cube", cube->serialNumber()));
            params.append(Param("serial number", radiatorThermostat->serialNumber()));
            params.append(Param("rf address", radiatorThermostat->rfAddress()));
            params.append(Param("room id", radiatorThermostat->roomId()));
            params.append(Param("room name", radiatorThermostat->roomName()));
            descriptor.setParams(params);
            descriptorList.append(descriptor);
        }
    }

    if(!descriptorList.isEmpty()){
        metaObject()->invokeMethod(this, "autoDevicesAppeared", Qt::QueuedConnection, Q_ARG(DeviceClassId, radiatorThermostateDeviceClassId), Q_ARG(QList<DeviceDescriptor>, descriptorList));
    }
}

void DevicePluginEQ3::updateCubeConfig()
{
    MaxCube *cube = static_cast<MaxCube*>(sender());
    Device *device;
    if (m_cubes.contains(cube)) {
        device = m_cubes.value(cube);
        device->setStateValue(portalEnabeldStateTypeId,cube->portalEnabeld());
        return;
    }
}

void DevicePluginEQ3::wallThermostatDataUpdated()
{
    MaxCube *cube = static_cast<MaxCube*>(sender());

    foreach (WallThermostat *wallThermostat, cube->wallThermostatList()) {
        foreach (Device *device, deviceManager()->findConfiguredDevices(wallThermostateDeviceClassId)){
            if(device->paramValue("serial number").toString() == wallThermostat->serialNumber()){
                device->setStateValue(confortTempStateTypeId, wallThermostat->confortTemp());
                device->setStateValue(ecoTempStateTypeId, wallThermostat->ecoTemp());
                device->setStateValue(maxSetpointTempStateTypeId, wallThermostat->maxSetPointTemp());
                device->setStateValue(minSetpointTempStateTypeId, wallThermostat->minSetPointTemp());
                device->setStateValue(errorOccuredStateTypeId, wallThermostat->errorOccured());
                device->setStateValue(initializedStateTypeId, wallThermostat->initialized());
                device->setStateValue(batteryLowStateTypeId, wallThermostat->batteryLow());
                device->setStateValue(linkStatusOKStateTypeId, wallThermostat->linkStatusOK());
                device->setStateValue(panelLockedStateTypeId, wallThermostat->panelLocked());
                device->setStateValue(gatewayKnownStateTypeId, wallThermostat->gatewayKnown());
                device->setStateValue(dtsActiveStateTypeId, wallThermostat->dtsActive());
                device->setStateValue(deviceModeStateTypeId, wallThermostat->deviceMode());
                device->setStateValue(deviceModeStringStateTypeId, wallThermostat->deviceModeString());
                device->setStateValue(setpointTempStateTypeId, wallThermostat->setpointTemperature());
                device->setStateValue(currentTemperatureStateTypeId, wallThermostat->currentTemperature());


            }
        }
    }
}

void DevicePluginEQ3::radiatorThermostatDataUpdated()
{
    MaxCube *cube = static_cast<MaxCube*>(sender());

    foreach (RadiatorThermostat *radiatorThermostat, cube->radiatorThermostatList()) {
        foreach (Device *device, deviceManager()->findConfiguredDevices(radiatorThermostateDeviceClassId)){
            if(device->paramValue("serial number").toString() == radiatorThermostat->serialNumber()){
                device->setStateValue(confortTempStateTypeId, radiatorThermostat->confortTemp());
                device->setStateValue(ecoTempStateTypeId, radiatorThermostat->ecoTemp());
                device->setStateValue(maxSetpointTempStateTypeId, radiatorThermostat->maxSetPointTemp());
                device->setStateValue(minSetpointTempStateTypeId, radiatorThermostat->minSetPointTemp());
                device->setStateValue(errorOccuredStateTypeId, radiatorThermostat->errorOccured());
                device->setStateValue(initializedStateTypeId, radiatorThermostat->initialized());
                device->setStateValue(batteryLowStateTypeId, radiatorThermostat->batteryLow());
                device->setStateValue(linkStatusOKStateTypeId, radiatorThermostat->linkStatusOK());
                device->setStateValue(panelLockedStateTypeId, radiatorThermostat->panelLocked());
                device->setStateValue(gatewayKnownStateTypeId, radiatorThermostat->gatewayKnown());
                device->setStateValue(dtsActiveStateTypeId, radiatorThermostat->dtsActive());
                device->setStateValue(deviceModeStateTypeId, radiatorThermostat->deviceMode());
                device->setStateValue(deviceModeStringStateTypeId, radiatorThermostat->deviceModeString());
                device->setStateValue(setpointTempStateTypeId, radiatorThermostat->setpointTemperature());
                device->setStateValue(offsetTempStateTypeId, radiatorThermostat->offsetTemp());
                device->setStateValue(windowOpenDurationStateTypeId, radiatorThermostat->windowOpenDuration());
                device->setStateValue(boostValueValueStateTypeId, radiatorThermostat->boostValveValue());
                device->setStateValue(boostDurationStateTypeId, radiatorThermostat->boostDuration());
                device->setStateValue(discalcWeekDayStateTypeId, radiatorThermostat->discalcingWeekDay());
                device->setStateValue(discalcTimeStateTypeId, radiatorThermostat->discalcingTime().toString("HH:mm"));
                device->setStateValue(valveMaximumSettingsStateTypeId, radiatorThermostat->valveMaximumSettings());
                device->setStateValue(valveOffsetStateTypeId, radiatorThermostat->valveOffset());
                device->setStateValue(valvePositionStateTypeId, radiatorThermostat->valvePosition());
            }
        }
    }
}
