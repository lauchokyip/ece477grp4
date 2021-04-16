// const queue = require("./queue.js");
const customerHelper = require("./helper/customerHelper.js");
const universalEmitter = require("./universalEmitter.js");
// const notification = require("./notification");

const store = {
    numPeople: 0,
    maxCapacity: 30, // 10 is easier for testing :)
};

/**
 * Initialize coreObject to default value.
 * Will be used to converted to JSON later
 */
function getMaxCapacity() {
    return store.maxCapacity;
}

function getNumPeople() {
    return store.numPeople;
}

/**
 * setStoreCapacity will set the capacity
 * @param {*} _capacity
 */
function setStoreCapacity(_capacity) {
    store["maxCapacity"] = _capacity;
    universalEmitter.emit("statusChange");
    universalEmitter.emit("numPeopleCapacityChange");
}

function setNumPeople(numPeople) {
    store["numPeople"] = numPeople;
    universalEmitter.emit("statusChange");
    universalEmitter.emit("numPeopleCapacityChange");
}

/**
 * Simple function to check if people can enter the store by checking the capacity
 * @param {} newNumPeople
 */
function isAllowToEnterStore(numPeople) {
    if (numPeople <= 0) /* Illegal input */ return false;
    if (numPeople + store["numPeople"] > store["maxCapacity"]) return false;
    return true;
}

// // barcodeScanned response JSON
// const barcodeScannedResponseJSON = {
//     ok: false,
//     customer: null,
//     isCheckingIn: false,
// };
/**
 * This function will perform the algorithm to check if the customer can enter the store or not.
 * Will put the customer on queue if the store has reached it capacity
 * will return JSON!!
 * @param {*} customer
 */
function enteringResponse(customer) {
    if (customerHelper.isInvalid(customer)) {
        return undefined;
    }

    var responseJSON = {};
    responseJSON.ok = true;
    responseJSON.customer = customer;
    responseJSON.isCheckingIn = true;

    return responseJSON;
}

function earlyEnterResponse(customer) {
    if (customerHelper.isInvalid(customer)) {
        return undefined;
    }

    var responseJSON = {};
    responseJSON.ok = true;
    responseJSON.customer = customer;
    responseJSON.isCheckingIn = false;

    return responseJSON;
}

function joinQueueResponse() {
    var responseJSON = {};
    responseJSON.ok = true;
    responseJSON.customer = customerHelper.makeEmptyCustomer();
    responseJSON.isCheckingIn = false;

    return responseJSON;
}

function errorResponse() {
    var responseJSON = {};
    responseJSON.ok = false;
    responseJSON.customer = customerHelper.makeEmptyCustomer();
    responseJSON.isCheckingIn = false;

    return responseJSON;
}

// TODO: notify function
/**
 * This will be triggered by IR
 */
// universalEmitter.on("leftStore", customerCheckingOut);
function leftStore() {
    if (store["numPeople"] == 0) {
        console.error("Error, no people in the store");
        return;
    }
    store["numPeople"]--;
    universalEmitter.emit("statusChange");
    universalEmitter.emit("numPeopleCapacityChange");
    // notifyNextCustomerOnQueue();
    // if there is no customer on Queue, do Nothing (?)
}

function enteredStore() {
    store["numPeople"]++;
    universalEmitter.emit("statusChange");
    universalEmitter.emit("numPeopleCapacityChange");
    if (store.numPeople > store.maxCapacity) {
        return false;
    }
    return true;
    // notifyNextCustomerOnQueue();
    // if there is no customer on Queue, do Nothing (?)
}

module.exports = {
    leftStore,
    enteredStore,
    enteringResponse,
    earlyEnterResponse,
    joinQueueResponse,
    errorResponse,
    setStoreCapacity,
    setNumPeople,
    isAllowToEnterStore,
    getMaxCapacity,
    getNumPeople,
};
