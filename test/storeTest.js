const storeApp = require('../store')
const queueApp = require('../queue')
const assert = require('chai').assert

describe("Simulating a Store, assuming maxCapacity is 10", () => {
    it('One customer checking in the store, bringing 1 people', () => {
       let retJSON = storeApp.customerCheckingIn(0, 2)
       assert.deepEqual(retJSON, JSON.stringify({
            ok: true,
            isToMicroController: true,
            customer: {
                id: 0,
                numOfPeople: 2,
                isTimeToCheckIn: true,
                isNewCustomer: false,
                isOnQueue: false,
                queueNum: -1,
            },
            errMsg: "",
       }))
       assert.equal(storeApp.store["numOfPeopleInStore"], 2)
       assert.equal(queueApp.queue.length, 0)
    })
    it('Second customer checking in the store, bringing 2 people', () => {
        let retJSON = storeApp.customerCheckingIn(5, 3)
        assert.deepEqual(retJSON, JSON.stringify({
             ok: true,
             isToMicroController: true,
             customer: {
                 id: 5,
                 numOfPeople: 3,
                 isTimeToCheckIn: true,
                 isNewCustomer: false,
                 isOnQueue: false,
                 queueNum: -1,
             },
             errMsg: "",
        }))
        assert.equal(storeApp.store["numOfPeopleInStore"], 5)
        assert.equal(queueApp.queue.length, 0)
     })
     it('Third customer checking in the store, bringing 4 people', () => {
        let retJSON = storeApp.customerCheckingIn(20, 5)
        assert.deepEqual(retJSON, JSON.stringify({
             ok: true,
             isToMicroController: true,
             customer: {
                 id: 20,
                 numOfPeople: 5,
                 isTimeToCheckIn: true,
                 isNewCustomer: false,
                 isOnQueue: false,
                 queueNum: -1,
             },
             errMsg: "",
        }))
        assert.equal(storeApp.store["numOfPeopleInStore"], 10)
        assert.equal(queueApp.queue.length, 0)
     })
     it('First customer checking out the store', () => {
        storeApp.customerCheckingOut()
        storeApp.customerCheckingOut()
        assert.equal(storeApp.store["numOfPeopleInStore"], 8)
        assert.equal(queueApp.queue.length, 0)
     })
     it('Fourth customer checking in the store, bringing 2 people', () => {
        let retJSON = storeApp.customerCheckingIn(25, 3)
        assert.deepEqual(retJSON, JSON.stringify({
             ok: true,
             isToMicroController: true,
             customer: {
                 id: 25,
                 numOfPeople: 3,
                 isTimeToCheckIn: false,
                 isNewCustomer: true,
                 isOnQueue: false,
                 queueNum: 0,
             },
             errMsg: "",
        }))
        assert.equal(storeApp.store["numOfPeopleInStore"], 8)
        assert.equal(queueApp.queue.length, 1)
     })
     it('Fifth customer checking in the store, not bringing anyone', () => {
        let retJSON = storeApp.customerCheckingIn(30, 1)
        assert.deepEqual(retJSON, JSON.stringify({
             ok: true,
             isToMicroController: true,
             customer: {
                 id: 30,
                 numOfPeople: 1,
                 isTimeToCheckIn: false,
                 isNewCustomer: true,
                 isOnQueue: false,
                 queueNum: 1,
             },
             errMsg: "",
        }))
        assert.equal(storeApp.store["numOfPeopleInStore"], 8)
        assert.equal(queueApp.queue.length, 2)
     })
     it('Fourth customer checking in the store (not his/her turn yet)', () => {
        let retJSON = storeApp.customerCheckingIn(25, 3)
        assert.deepEqual(retJSON, JSON.stringify({
             ok: true,
             isToMicroController: true,
             customer: {
                 id: 25,
                 numOfPeople: 3,
                 isTimeToCheckIn: false,
                 isNewCustomer: false,
                 isOnQueue: true,
                 queueNum: 0,
             },
             errMsg: "",
        }))
        assert.equal(storeApp.store["numOfPeopleInStore"], 8)
        assert.equal(queueApp.queue.length, 2)
     })
     it('Ffith customer checking in the store (not his/her turn yet)', () => {
        let retJSON = storeApp.customerCheckingIn(30, 1)
        assert.deepEqual(retJSON, JSON.stringify({
             ok: true,
             isToMicroController: true,
             customer: {
                 id: 30,
                 numOfPeople: 1,
                 isTimeToCheckIn: false,
                 isNewCustomer: false,
                 isOnQueue: true,
                 queueNum: 1,
             },
             errMsg: "",
        }))
        assert.equal(storeApp.store["numOfPeopleInStore"], 8)
        assert.equal(queueApp.queue.length, 2)
     })
     it('Second customer checking out the store, Fourth customer will be notified', () => {
        storeApp.customerCheckingOut()
        storeApp.customerCheckingOut()
        storeApp.customerCheckingOut()
        assert.equal(storeApp.store["numOfPeopleInStore"], 5)
        assert.equal(queueApp.queue.length, 2)
     })
     it('Fifth customer checking in the store (not his/her turn yet)', () => {
        let retJSON = storeApp.customerCheckingIn(30, 1)
        assert.deepEqual(retJSON, JSON.stringify({
             ok: true,
             isToMicroController: true,
             customer: {
                 id: 30,
                 numOfPeople: 1,
                 isTimeToCheckIn: false,
                 isNewCustomer: false,
                 isOnQueue: true,
                 queueNum: 1,
             },
             errMsg: "",
        }))
        assert.equal(storeApp.store["numOfPeopleInStore"], 5)
        assert.equal(queueApp.queue.length, 2)
     })
     it('Fourth customer checking in the store, bringing 2 people, Notify fifth customer', () => {
        let retJSON = storeApp.customerCheckingIn(25, 3)
        assert.deepEqual(retJSON, JSON.stringify({
             ok: true,
             isToMicroController: true,
             customer: {
                 id: 25,
                 numOfPeople: 3,
                 isTimeToCheckIn: true,
                 isNewCustomer: false,
                 isOnQueue: false,
                 queueNum: -1,
             },
             errMsg: "",
        }))
        assert.equal(storeApp.store["numOfPeopleInStore"], 8)
        assert.equal(queueApp.queue.length, 1)
     })
     it('Sixth customer checking in the store, not bringing anyone (not his/her turn yet)', () => {
        let retJSON = storeApp.customerCheckingIn(6, 1)
        assert.deepEqual(retJSON, JSON.stringify({
             ok: true,
             isToMicroController: true,
             customer: {
                 id: 6,
                 numOfPeople: 1,
                 isTimeToCheckIn: false,
                 isNewCustomer: true,
                 isOnQueue: false,
                 queueNum: 1,
             },
             errMsg: "",
        }))
        assert.equal(storeApp.store["numOfPeopleInStore"], 8)
        assert.equal(queueApp.queue.length, 2)
     })
     it('Fifth customer checking in the store, notify sixth customer', () => {
        let retJSON = storeApp.customerCheckingIn(30, 1)
        assert.deepEqual(retJSON, JSON.stringify({
             ok: true,
             isToMicroController: true,
             customer: {
                 id: 30,
                 numOfPeople: 1,
                 isTimeToCheckIn: true,
                 isNewCustomer: false,
                 isOnQueue: false,
                 queueNum: -1,
             },
             errMsg: "",
        }))
        assert.equal(storeApp.store["numOfPeopleInStore"], 9)
        assert.equal(queueApp.queue.length, 1)
     })
     it('Sixth customer checking in the store (Shop is MAX)', () => {
        let retJSON = storeApp.customerCheckingIn(6, 1)
        assert.deepEqual(retJSON, JSON.stringify({
             ok: true,
             isToMicroController: true,
             customer: {
                 id: 6,
                 numOfPeople: 1,
                 isTimeToCheckIn: true,
                 isNewCustomer: false,
                 isOnQueue: false,
                 queueNum: -1,
             },
             errMsg: "",
        }))
        assert.equal(storeApp.store["numOfPeopleInStore"], 10)
        assert.equal(queueApp.queue.length, 0)
     })
     it('Store is closing, Everyone is leaving the store', () => {
        for(let i = 0; i < 10; i++)
        {
            storeApp.customerCheckingOut()
        }
        assert.equal(storeApp.store["numOfPeopleInStore"], 0)
        assert.equal(queueApp.queue.length, 0)
    })
     it('Invalid Customer ID', () => {
        assert.equal(storeApp.customerCheckingIn(-1, 100), undefined)
     })
     it('Invalid Number Of People', () => {
        assert.equal(storeApp.customerCheckingIn(1, -1), undefined)
    })

})