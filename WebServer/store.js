const queue = require("./queue.js");
const customerHelper = require("./helper/customerHelper.js");
const notification = require("./notification");

const store = {
    numPeopleInStore: 0,
    maxCapacity: 10, // 10 is easier for testing :)
};

var lastNotifyCustomerID;

/**
 * Initialize coreObject to default value.
 * Will be used to converted to JSON later
 */
function generateNewCoreObject() {
    return {
        ok: false,
        isToMicroController: false,
        customer: {
            id: null,
            numPeople: 0,
            isTimeToCheckIn: false,
            isNewCustomer: false,
            isOnQueue: false,
            queueNum: -1,
        },
        errMsg: "",
    };
}

function setCoreObjectOuter(coreObject, param, value) {
    if (coreObject == undefined) {
        console.error("core Object is undefined");
        return;
    }
    if (
        param !== "ok" &&
        param !== "isToMicroController" &&
        param !== "errMsg"
    ) {
        console.error("parameter is wrong");
        return;
    }
    coreObject[param] = value;
}

function setCoreObjectCustomer(coreObject, param, value) {
    if (coreObject == undefined) {
        console.error("core Object is undefined");
        return;
    }
    if (
        param !== "id" &&
        param !== "numPeople" &&
        param !== "isNewCustomer" &&
        param !== "isTimeToCheckIn" &&
        param !== "isOnQueue" &&
        param !== "queueNum"
    ) {
        console.error("parameter is wrong");
        return;
    }
    coreObject["customer"][param] = value;
}

/**
 * setStoreCapacity will set the capacity
 * @param {*} _capacity
 */
function setStoreCapacity(_capacity) {
    store["maxCapacity"] = _capacity;
}

/**
 * Simple function to check if people can enter the store by checking the capacity
 * @param {} newNumPeople
 */
function isAllowToEnterStore(newNumPeople) {
    if (newNumPeople <= 0) /* Illegal input */ return false;
    if (newNumPeople + store["numPeopleInStore"] > store["maxCapacity"])
        return false;
    return true;
}

function addCustomerToQueueAndModifyCoreObject(newCoreObject, customer) {
    if (newCoreObject == undefined) {
        console.error("core object is undefined");
        return;
    }

    const oldQueueLen = queue.length();
    const ret = queue.addCustomerToQueue(customer);
    const newQueueLen = queue.length();
    // if the customer is already on the queue, if so, return the queueNum
    if (oldQueueLen == newQueueLen) {
        setCoreObjectOuter(newCoreObject, "ok", true);
        setCoreObjectOuter(newCoreObject, "isToMicroController", true);
        setCoreObjectCustomer(newCoreObject, "id", customer.id);
        setCoreObjectCustomer(newCoreObject, "numPeople", customer.numPeople);
        setCoreObjectCustomer(newCoreObject, "isOnQueue", true);
        setCoreObjectCustomer(newCoreObject, "queueNum", ret);
    }
    // if the customer is not on the queue, add Customer to the queue (new Customer)
    else {
        setCoreObjectOuter(newCoreObject, "ok", true);
        setCoreObjectOuter(newCoreObject, "isToMicroController", true);
        setCoreObjectCustomer(newCoreObject, "id", customer.id);
        setCoreObjectCustomer(newCoreObject, "numPeople", customer.numPeople);
        setCoreObjectCustomer(newCoreObject, "isNewCustomer", true);
        setCoreObjectCustomer(newCoreObject, "queueNum", ret);
    }
}

/**
 * This function will perform the algorithm to check if the customer can enter the store or not.
 * Will put the customer on queue if the store has reached it capacity
 * will return JSON!!
 * @param {*} customer
 */
function customerCheckingIn(customer) {
    var id = customer.id;
    var numPeople = customer.numPeople;
    if (
        !customerHelper.checkID(id) ||
        !customerHelper.checkNumPeople(numPeople)
    )
        return undefined;

    const newCoreObject = generateNewCoreObject();
    // check if the customer can enter the store
    // if the customer can enter the store,
    if (isAllowToEnterStore(numPeople)) {
        // if there is nobody waiting on the queue, simply update the numOfStoreCapacity
        if (queue.length() == 0) {
            store["numPeopleInStore"] += numPeople;
            setCoreObjectOuter(newCoreObject, "ok", true);
            setCoreObjectOuter(newCoreObject, "isToMicroController", true);
            setCoreObjectCustomer(newCoreObject, "id", id);
            setCoreObjectCustomer(newCoreObject, "numPeople", numPeople);
            setCoreObjectCustomer(newCoreObject, "isTimeToCheckIn", true);

            return JSON.stringify(newCoreObject);
        }
        // can enter and there is somebody in the queue!
        // make sure he/she is the right person (top on the queue)
        // delete them from the notification Queue
        if (queue.get(0).customer.id === id) {
            customer = queue.deleteCustomerFromQueue(0);
            if(notification.deleteSubsctiptionByID(customer.id) !== true)
            {
                console.log("Error occured while deleting customer from Notification queue")
            }

            store["numPeopleInStore"] += numPeople;
            setCoreObjectOuter(newCoreObject, "ok", true);
            setCoreObjectOuter(newCoreObject, "isToMicroController", true);
            setCoreObjectCustomer(newCoreObject, "id", id);
            setCoreObjectCustomer(newCoreObject, "numPeople", numPeople);
            setCoreObjectCustomer(newCoreObject, "isTimeToCheckIn", true);

            // if the store is still not full, notify the next customer
            notifyNextCustomerOnQueue()

            return JSON.stringify(newCoreObject);
        }
        // he/she is not on the top of the queue, add them on the queue
        else {
            addCustomerToQueueAndModifyCoreObject(newCoreObject, customer);
            return JSON.stringify(newCoreObject);
        }
    }
    // if the customer cannot enter the store,
    else {
        addCustomerToQueueAndModifyCoreObject(newCoreObject, customer);
        return JSON.stringify(newCoreObject);
    }
}

// TODO: notify function

/**
 * This will be triggered by IR
 */
function customerCheckingOut() {
    if (store["numPeopleInStore"] == 0) {
        console.error("Error, no people in the store");
        return;
    }
    store["numPeopleInStore"]--;
    notifyNextCustomerOnQueue()
    // if there is no customer on Queue, do Nothing (?)
}

/**
 * notifyNextCustomerOnQueue will notify the next customer on queue if and only if there is customer on the queue
 */
function notifyNextCustomerOnQueue(){
 // if there is customer waiting on Queue, notify them
 if (queue.length() > 0) {
    // check if the customer and his/her friends can enter
    // if allows, delete them from queue, otherwise WAIT
    const nextCustomer = queue.get(0);
    if (isAllowToEnterStore(nextCustomer.customer.numPeople) && nextCustomer.customer.id !== lastNotifyCustomerID) {
        lastNotifyCustomerID = nextCustomer.customer.id
        //TODO: make notification function better
        notification.triggerPushMsgByID(nextCustomer.customer.id)
        .then((data) => {
            lastNotifyCustomerID = nextCustomer.customer.id
            if(data === true)
                console.log("Notification was succesfully sent to user " + nextCustomer.id)
            else
                console.log(data)
        })
    }
}
}

module.exports = {
    store,
    customerCheckingIn,
    customerCheckingOut,
    setStoreCapacity,
};
