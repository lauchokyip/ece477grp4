const store = require("./store.js");
const universalEmitter = require("./universalEmitter.js");

const TIME_TO_ENTER = 5 * 60 * 1000;

var IDLE = true;
const queue = [];

/**
 * customerIndexOnQueue will check if the customer is on Queue or not
 * return the array index if customer is found, otherwise return -1
 * @param {*} customerId
 */
function customerIndexOnQueue(customerId) {
    return queue.findIndex(
        (arrayElement) => arrayElement.customer.id === customerId
    );
}

function idExistsInQueue(id) {
    if (queue.length === 0) {
        return false;
    }
    queue.forEach((qNode) => {
        if (qNode.customer.id === id) {
            return true;
        }
    });
    return false;
}

/**
 * adds socket id to the queue node of a the customer at index.
 * @param {*} index
 * @param {*} socketID
 */
function addSocketIDToIndex(index, socketID) {
    if (index < 0 || index >= queue.length) {
        return false;
    }

    queue[index].socketID = socketID;
    return true;
}

function addSubscriptionObjToId(id, subObj) {
    var index = customerIndexOnQueue(id);
    if (index === -1) {
        return false;
    }
    queue[index].subObj = subObj;
    return true;
}

/**
 * returns a array of socket IDs in the of the customers in the queue
 */
function getSocketIDs() {
    var socketIDArr = [];
    queue.forEach((element) => {
        socketIDArr.push(element.socketID);
    });
    return socketIDArr;
}

function getSocketID(index) {
    if (index < 0 || index >= queue.length) {
        return undefined;
    }
    return queue[index].socketID;
}

function getAllSubcriptionObj() {
    var subObjArr = [];
    queue.forEach((queueNode) => {
        subObjArr.push(queueNode.subObj);
    });
    return subObjArr;
}

function getSubcriptionObj(index) {
    if (index < 0 || index >= queue.length) {
        return null;
    }
    return queue[index].subObj;
}

/**
 *  addCustomerToQueue will add the customer to the queue given id,numOfPeople
 *  return the array index if customer is already on the queue, otherwise returns the index of the new added customer
 * @param {*} customer
 */
function addCustomerToQueue(customer) {
    const ret = customerIndexOnQueue(customer.id);
    if (ret !== -1) return ret;
    const timestamp = Date.now();
    queue.push({
        customer,
        timestamp,
    });
    universalEmitter.emit("queueChange");
    universalEmitter.emit("statusChange");

    // add the code for what happens when there is enough space for people to enter when someone joined the queue
    return queue.length - 1;
}

/**
 * deleteCustomerFromQueue will delete customer from the queue given the array index
 * return undefined if customer is not on the queue or any error occur when perform deletion,
 * return the customer object if the customer is deleted
 * @param {*} arrayIndex
 */
function deleteCustomerAtIndex(arrayIndex) {
    if (arrayIndex < 0 || arrayIndex + 1 > queue.length) return undefined;
    var customer = queue.splice(arrayIndex, 1);
    if (arrayIndex === 0) universalEmitter.emit("queueFirstRemoved");
    universalEmitter.emit("statusChange");
    universalEmitter.emit("queueChange");
    return customer;
}

function deleteCustomerWithID(customerID) {
    const pos = customerIndexOnQueue(customerID);
    if (pos === -1) return undefined;
    return deleteCustomerAtIndex(pos);
}

function checkInCustomer() {
    IDLE = false;
    clearTimeout(checkInTimeout);
    return deleteCustomerAtIndex(0)[0];
}

universalEmitter.on("leftStore", canCheckInHandler);
universalEmitter.on("queueChange", canCheckInHandler);
universalEmitter.on("queueFirstRemoved", canCheckInHandler);
universalEmitter.on("numPeopleCapacityChange", canCheckInHandler);
universalEmitter.on("setIDLE", canCheckInHandler);
function canCheckInHandler() {
    if (firstCustomerCanEnter()) {
        startCheckIn();
    } else {
        if (queue.length > 0) {
            delete queue[0].checkInTimer;
            universalEmitter.emit("updateFlag");
        }
    }
}

var checkInTimeout;
function startCheckIn() {
    if (hasCheckInTimerStarted()) {
        return;
    }
    var timestamp = Date.now();
    var timeToEnter = timestamp + TIME_TO_ENTER;
    var checkInTimer = {
        timestamp,
        timeToEnter,
    };
    queue[0].checkInTimer = checkInTimer;
    checkInTimeout = setTimeout(function () {
        deleteCustomerAtIndex(0);
    }, timeToEnter - timestamp);
    universalEmitter.emit("startedCheckIn");
}

function hasCheckInTimerStarted() {
    return (
        queue[0].checkInTimer !== null && queue[0].checkInTimer !== undefined
    );
}

function getCheckInTimer() {
    if (queue.length === 0) {
        return undefined;
    }
    if (!hasCheckInTimerStarted()) {
        return undefined;
    }
    return queue[0].checkInTimer;
}

function firstCustomerCanEnter() {
    if (queue.length === 0) {
        return false;
    }
    return store.isAllowToEnterStore(queue[0].customer.numPeople) && IDLE;
}

function length() {
    return queue.length;
}

function get(index) {
    if (index < 0 || index >= queue.length) {
        console.log(`Invalid get to queue at index ${index}`);
        return undefined;
    }
    return queue[index];
}

function setIDLE() {
    IDLE = true;
    universalEmitter.emit("setIDLE");
}

function getFullQueue() {
    var simpQueue = [];
    queue.forEach((element) => {
        simpQueue.push(element.customer);
    });
    return simpQueue;
}

function print() {
    queue.forEach((element) => {
        console.log(element);
    });
}

module.exports = {
    get,
    length,
    customerIndexOnQueue,
    addCustomerToQueue,
    deleteCustomerAtIndex,
    deleteCustomerWithID,
    addSocketIDToIndex,
    getSocketIDs,
    getSocketID,
    firstCustomerCanEnter,
    checkInCustomer,
    getCheckInTimer,
    idExistsInQueue,
    getFullQueue,
    addSubscriptionObjToId,
    getSubcriptionObj,
    getAllSubcriptionObj,
    print,
    setIDLE,
};
