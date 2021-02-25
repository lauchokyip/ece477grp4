const queueApp = require("./queue")
const store = {
    numOfPeopleInStore: 0,
    maxCapacity: 10 // 10 is easier for testing :)
}

/**
 * Initialize coreObject to default value.
 * Will be used to converted to JSON later
 */
function generateNewCoreObject() {
    return {
        ok: false,
        isToMicroController: false,
        customer: {
            id: -1,
            numOfPeople: 0,
            isTimeToCheckIn: false,
            isNewCustomer: false,
            isOnQueue: false,
            queueNum: -1,
        },
        errMsg: "",
    }
} 

function setCoreObjectOuter(coreObject, param, value) {
    if (coreObject == undefined)
    {
        console.error("core Object is undefined");
        return
    }    
    if (param !== "ok" && param !== "isToMicroController" && param !== "errMsg")
    {
        console.error("parameter is wrong")
        return
    }
    coreObject[param] = value
}

function setCoreObjectCustomer(coreObject, param, value) {
    if (coreObject == undefined)
    {
        console.error("core Object is undefined");
        return 
    }    
    if (param !== "id" && param !== "numOfPeople" && param !== "isNewCustomer" && 
        param !== "isTimeToCheckIn" && param !== "isOnQueue" && param !== "queueNum"
        )
    {
        console.error("parameter is wrong")
        return
    }
    coreObject["customer"][param] = value
}

/**
 * setStoreCapacity will set the capacity 
 * @param {*} _capacity 
 */
function setStoreCapacity(_capacity) {
    store["maxCapacity"] = _capacity
}

/**
 * Simple function to check if people can enter the store by checking the capacity
 * @param {} newNumOfPeople 
 */
function isAllowToEnterStore( newNumOfPeople) {
    if (newNumOfPeople <= 0) /* Illegal input */
        return false
    if (newNumOfPeople + store["numOfPeopleInStore"] > store["maxCapacity"])
        return false
    return true
}


function addCustomerToQueueAndModifyCoreObject(newCoreObject, id, numOfPeople) {
    if(newCoreObject == undefined)
    {
        console.error("core object is undefined")
        return
    }

    const oldQueueLen = queueApp.queue.length
    const ret = queueApp.addCustomerToQueue(id, numOfPeople)
    const newQueueLen = queueApp.queue.length
    // if the customer is already on the queue, if so, return the queueNum
    if(oldQueueLen == newQueueLen)
    {
        setCoreObjectOuter(newCoreObject, "ok", true)
        setCoreObjectOuter(newCoreObject, "isToMicroController", true)
        setCoreObjectCustomer(newCoreObject, "id", id)
        setCoreObjectCustomer(newCoreObject,"numOfPeople", numOfPeople)
        setCoreObjectCustomer(newCoreObject, "isOnQueue", true)
        setCoreObjectCustomer(newCoreObject, "queueNum", ret)
    }
    // if the customer is not on the queue, add Customer to the queue (new Customer)
    else
    {
        setCoreObjectOuter(newCoreObject, "ok", true)
        setCoreObjectOuter(newCoreObject, "isToMicroController", true)
        setCoreObjectCustomer(newCoreObject, "id", id)
        setCoreObjectCustomer(newCoreObject,"numOfPeople", numOfPeople)
        setCoreObjectCustomer(newCoreObject, "isNewCustomer", true)
        setCoreObjectCustomer(newCoreObject, "queueNum", ret)
    }
}


/**
 * This function will perform the algorithm to check if the customer can enter the store or not.
 * Will put the customer on queue if the store has reached it capacity
 * will return JSON!!
 * @param {*} id 
 * @param {*} numOfPeople 
 */
function customerCheckingIn(id, numOfPeople) {
    if (id < 0|| numOfPeople <= 0)
        return undefined

    const newCoreObject = generateNewCoreObject()
    // check if the customer can enter the store
    // if the customer can enter the store,
    if(isAllowToEnterStore(numOfPeople))
    {
        // if there is nobody waiting on the queue, simply update the numOfStoreCapacity
        if (queueApp.queue.length == 0) {
            store["numOfPeopleInStore"] += numOfPeople
            setCoreObjectOuter(newCoreObject, "ok", true)
            setCoreObjectOuter(newCoreObject, "isToMicroController", true)
            setCoreObjectCustomer(newCoreObject, "id", id)
            setCoreObjectCustomer(newCoreObject,"numOfPeople", numOfPeople)
            setCoreObjectCustomer(newCoreObject, "isTimeToCheckIn", true)
            
            return JSON.stringify(newCoreObject)
        }
        // can enter and there is somebody in the queue! 
        // make sure he/she is the right person (top on the queue)
        if (queueApp.queue[0].id === id)
        {
            queueApp.deleteCustomerFromQueue(0)
            
            store["numOfPeopleInStore"] += numOfPeople
            setCoreObjectOuter(newCoreObject, "ok", true)
            setCoreObjectOuter(newCoreObject, "isToMicroController", true)
            setCoreObjectCustomer(newCoreObject, "id", id)
            setCoreObjectCustomer(newCoreObject,"numOfPeople", numOfPeople)
            setCoreObjectCustomer(newCoreObject, "isTimeToCheckIn", true)

            return JSON.stringify(newCoreObject)
        }
        // he/she is not on the top of the queue, add them on the queue
        else 
        {
            addCustomerToQueueAndModifyCoreObject(newCoreObject,id, numOfPeople)
            return JSON.stringify(newCoreObject)
        }
    }
    // if the customer cannot enter the store,
    else
    {
      addCustomerToQueueAndModifyCoreObject(newCoreObject,id, numOfPeople)
      return JSON.stringify(newCoreObject)
    } 
}

// TODO: notify function

/**
 * This will be triggered by IR
 */
function customerCheckingOut() {
    if (store["numOfPeopleInStore"] == 0)
    {
        console.error("Error, no people in the store")
        return 
    }
    store["numOfPeopleInStore"]--
    // if there is customer waiting on Queue, notify them
    if(queueApp.queue.length > 0)
    {
        // check if the customer and his/her friends can enter
        // if allows, delete them from queue, otherwise WAIT
        const nextCustomer = queueApp.queue[0]
        if(isAllowToEnterStore(nextCustomer.numOfPeople))
        {
            //TODO: notify function goes here
           
        }
    }
    // if there is no customer on Queue, do Nothing (?)
}


module.exports = {
    store,
    customerCheckingIn,
    customerCheckingOut,
    setStoreCapacity,
}