const storeApp = require("../store");
const queue = require("../queue.js");
const notification = require("../notification.js")
const assert = require("chai").assert;
const customerHelper = require("../helper/customerHelper.js");

describe("Simulating a Store, assuming maxCapacity is 10", () => {
    it("One customer checking in the store, bringing 1 people", () => {
        let retJSON = storeApp.customerCheckingIn(makeCust(0, 2));
        assert.deepEqual(
            retJSON,
            JSON.stringify({
                ok: true,
                isToMicroController: true,
                customer: {
                    id: "00000000",
                    numPeople: 2,
                    isTimeToCheckIn: true,
                    isNewCustomer: false,
                    isOnQueue: false,
                    queueNum: -1,
                },
                errMsg: "",
            })
        );
        assert.equal(storeApp.store["numPeopleInStore"], 2);
        assert.equal(queue.length(), 0);
    });
    it("Second customer checking in the store, bringing 2 people", () => {
        let retJSON = storeApp.customerCheckingIn(makeCust(5, 3));
        assert.deepEqual(
            retJSON,
            JSON.stringify({
                ok: true,
                isToMicroController: true,
                customer: {
                    id: "00000005",
                    numPeople: 3,
                    isTimeToCheckIn: true,
                    isNewCustomer: false,
                    isOnQueue: false,
                    queueNum: -1,
                },
                errMsg: "",
            })
        );
        assert.equal(storeApp.store["numPeopleInStore"], 5);
        assert.equal(queue.length(), 0);
    });
    it("Third customer checking in the store, bringing 4 people", () => {
        let retJSON = storeApp.customerCheckingIn(makeCust(20, 5));
        assert.deepEqual(
            retJSON,
            JSON.stringify({
                ok: true,
                isToMicroController: true,
                customer: {
                    id: "00000014",
                    numPeople: 5,
                    isTimeToCheckIn: true,
                    isNewCustomer: false,
                    isOnQueue: false,
                    queueNum: -1,
                },
                errMsg: "",
            })
        );
        assert.equal(storeApp.store["numPeopleInStore"], 10);
        assert.equal(queue.length(), 0);
    });
    it("First customer checking out the store", () => {
        storeApp.customerCheckingOut();
        storeApp.customerCheckingOut();
        assert.equal(storeApp.store["numPeopleInStore"], 8);
        assert.equal(queue.length(), 0);
    });
    it("Fourth customer checking in the store, bringing 2 people", () => {
        let retJSON = storeApp.customerCheckingIn(makeCust(25, 3));
        assert.deepEqual(
            retJSON,
            JSON.stringify({
                ok: true,
                isToMicroController: true,
                customer: {
                    id: "00000019",
                    numPeople: 3,
                    isTimeToCheckIn: false,
                    isNewCustomer: true,
                    isOnQueue: false,
                    queueNum: 0,
                },
                errMsg: "",
            })
        );
        assert.equal(storeApp.store["numPeopleInStore"], 8);
        assert.equal(queue.length(), 1);
    });
    it("Fifth customer checking in the store, not bringing anyone", () => {
        let retJSON = storeApp.customerCheckingIn(makeCust(30, 1));
        assert.deepEqual(
            retJSON,
            JSON.stringify({
                ok: true,
                isToMicroController: true,
                customer: {
                    id: "0000001e",
                    numPeople: 1,
                    isTimeToCheckIn: false,
                    isNewCustomer: true,
                    isOnQueue: false,
                    queueNum: 1,
                },
                errMsg: "",
            })
        );
        assert.equal(storeApp.store["numPeopleInStore"], 8);
        assert.equal(queue.length(), 2);
    });
    it("Fourth customer checking in the store (not his/her turn yet)", () => {
        let retJSON = storeApp.customerCheckingIn(makeCust(25, 3));
        assert.deepEqual(
            retJSON,
            JSON.stringify({
                ok: true,
                isToMicroController: true,
                customer: {
                    id: "00000019",
                    numPeople: 3,
                    isTimeToCheckIn: false,
                    isNewCustomer: false,
                    isOnQueue: true,
                    queueNum: 0,
                },
                errMsg: "",
            })
        );
        assert.equal(storeApp.store["numPeopleInStore"], 8);
        assert.equal(queue.length(), 2);
    });
    it("Ffith customer checking in the store (not his/her turn yet)", () => {
        let retJSON = storeApp.customerCheckingIn(makeCust(30, 1));
        assert.deepEqual(
            retJSON,
            JSON.stringify({
                ok: true,
                isToMicroController: true,
                customer: {
                    id: "0000001e",
                    numPeople: 1,
                    isTimeToCheckIn: false,
                    isNewCustomer: false,
                    isOnQueue: true,
                    queueNum: 1,
                },
                errMsg: "",
            })
        );
        assert.equal(storeApp.store["numPeopleInStore"], 8);
        assert.equal(queue.length(), 2);
    });
    it("Second customer checking out the store, Fourth customer will be notified", () => {
        storeApp.customerCheckingOut();
        storeApp.customerCheckingOut();
        storeApp.customerCheckingOut();
        assert.equal(storeApp.store["numPeopleInStore"], 5);
        assert.equal(queue.length(), 2);
    });
    it("Fifth customer checking in the store (not his/her turn yet)", () => {
        let retJSON = storeApp.customerCheckingIn(makeCust(30, 1));
        assert.deepEqual(
            retJSON,
            JSON.stringify({
                ok: true,
                isToMicroController: true,
                customer: {
                    id: "0000001e",
                    numPeople: 1,
                    isTimeToCheckIn: false,
                    isNewCustomer: false,
                    isOnQueue: true,
                    queueNum: 1,
                },
                errMsg: "",
            })
        );
        assert.equal(storeApp.store["numPeopleInStore"], 5);
        assert.equal(queue.length(), 2);
    });
    it("Fourth customer checking in the store, bringing 2 people, Notify fifth customer", () => {
        let retJSON = storeApp.customerCheckingIn(makeCust(25, 3));
        assert.deepEqual(
            retJSON,
            JSON.stringify({
                ok: true,
                isToMicroController: true,
                customer: {
                    id: "00000019",
                    numPeople: 3,
                    isTimeToCheckIn: true,
                    isNewCustomer: false,
                    isOnQueue: false,
                    queueNum: -1,
                },
                errMsg: "",
            })
        );
        assert.equal(storeApp.store["numPeopleInStore"], 8);
        assert.equal(queue.length(), 1);
    });
    it("Sixth customer checking in the store, not bringing anyone (not his/her turn yet)", () => {
        let retJSON = storeApp.customerCheckingIn(makeCust(6, 1));
        assert.deepEqual(
            retJSON,
            JSON.stringify({
                ok: true,
                isToMicroController: true,
                customer: {
                    id: "00000006",
                    numPeople: 1,
                    isTimeToCheckIn: false,
                    isNewCustomer: true,
                    isOnQueue: false,
                    queueNum: 1,
                },
                errMsg: "",
            })
        );
        assert.equal(storeApp.store["numPeopleInStore"], 8);
        assert.equal(queue.length(), 2);
    });
    it("Fifth customer checking in the store, notify sixth customer", () => {
        let retJSON = storeApp.customerCheckingIn(makeCust(30, 1));
        assert.deepEqual(
            retJSON,
            JSON.stringify({
                ok: true,
                isToMicroController: true,
                customer: {
                    id: "0000001e",
                    numPeople: 1,
                    isTimeToCheckIn: true,
                    isNewCustomer: false,
                    isOnQueue: false,
                    queueNum: -1,
                },
                errMsg: "",
            })
        );
        assert.equal(storeApp.store["numPeopleInStore"], 9);
        assert.equal(queue.length(), 1);
    });
    it("Sixth customer checking in the store (Shop is MAX)", () => {
        let retJSON = storeApp.customerCheckingIn(makeCust(6, 1));
        assert.deepEqual(
            retJSON,
            JSON.stringify({
                ok: true,
                isToMicroController: true,
                customer: {
                    id: "00000006",
                    numPeople: 1,
                    isTimeToCheckIn: true,
                    isNewCustomer: false,
                    isOnQueue: false,
                    queueNum: -1,
                },
                errMsg: "",
            })
        );
        assert.equal(storeApp.store["numPeopleInStore"], 10);
        assert.equal(queue.length(), 0);
    });
    it("Store is closing, Everyone is leaving the store", () => {
        for (let i = 0; i < 10; i++) {
            storeApp.customerCheckingOut();
        }
        assert.equal(storeApp.store["numPeopleInStore"], 0);
        assert.equal(queue.length(), 0);
    });
    it("Invalid Customer ID", () => {
        let customerInvalidID = makeCust(0, 4);
        customerInvalidID.id = "123456789";
        assert.equal(storeApp.customerCheckingIn(customerInvalidID), undefined);
    });
    it("Invalid Number Of People", () => {
        assert.equal(storeApp.customerCheckingIn(makeCust(1, -1)), undefined);
    });
});

function makeCust(id, numPeople) {
    var hexID = ("00000000" + id.toString(16)).slice(
        -1 * (customerHelper.ID_BITLEN / 4)
    );
    return customerHelper.make(
        customerHelper.makePoten("trial", "trial@trail.com", numPeople),
        hexID
    );
}
