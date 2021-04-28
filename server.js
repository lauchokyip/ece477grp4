// Package level import
const express = require("express");
const http = require("http");
const socket = require("socket.io");
const bodyParser = require("body-parser");

// User-defined import
const potentialQueue = require("./potentialQueue.js");
const serverSocket = require("./serverSocket.js");
const queue = require("./queue.js");
const notification = require("./notification");
const customerHelper = require("./helper/customerHelper.js");
const store = require("./store.js");
const errorMgmt = require("./errorMgmt.js");

const PORT = process.env.PORT || 3001;
const app = express();
const httpServer = http.Server(app);
const io = socket(httpServer);

const universalEmitter = require("./universalEmitter.js");
// global.universalEmitter = new events();

//variables
const STORE_SECRET = "grp4";
const circumvent = true;

// middleware
app.use(express.static("public"));
app.use("/helper", express.static("helper"));
app.use("/bootstrap", express.static("node_modules/bootstrap/dist/css"));
app.use(express.json({ limit: "1mb" }));
app.use(bodyParser.json());
app.set("view engine", "ejs");

io.sockets.on("connection", serverSocket.connection);

// -----------------PAGE REQUESTS----------------------x
app.get("/", (req, res) => {
    res.render("index");
});

app.get("/joinQueue", (req, res) => {
    res.render("joinQueue");
});

app.get("/register", (req, res) => {
    res.render("registerNotification");
});

app.get("/queue", (req, res) => {
    res.render("clientQueue");
});

app.get("/endScreen", (req, res) => {
    res.render("endScreen");
});

app.get("/consoleLogin", (req, res) => {
    res.render("consoleLogin");
});

app.get("/console", (req, res) => {
    res.render("console");
});

// test admin push Route
app.get("/adminTestPush", async (req, res) => {
    const payload = JSON.stringify({
        title: "Broadcasting messages to everyone",
    });
    const subObjArr = queue.getAllSubcriptionObj();
    // console.log(subObjArr);
    queue.print();
    notification.triggerPushMsgArr(subObjArr, payload);
    res.status(200);
    res.json({ success: true });
});

// test pushing to single user
app.get("/testPush", (req, res) => {
    const id = req.query.id;
    const payload = JSON.stringify({
        title: `Sending message to i: ${id}`,
    });
    const index = queue.customerIndexOnQueue(id);
    const subObj = queue.getSubcriptionObj(index);
    if (subObj === null || subObj === undefined) {
        res.status(400);
        res.json({ success: false });
        return;
    }
    notification.triggerPushMsg(subObj, payload);

    res.status(200);
    res.json({ success: true });
});

// ----------------------------------------------------x
// -----------------POST REQUESTS----------------------x
// Subscribe Route
app.post("/subscribe", (req, res) => {
    if (!notification.isValidSaveRequest(req, res)) {
        return;
    }
    const subObj = req.body.subscription;
    const id = req.body.id;
    console.log("Subscribing: ", req.body);

    queue.addSubscriptionObjToId(id, subObj);

    res.status(200);
});

app.post("/getPotenID", (req, res) => {
    const potenCustomer = req.body.potenCustomer;
    const potenID = potentialQueue.generateID(potenCustomer);
    if (potenID === null) {
        res.status(500);
        console.log("Something went wrong generating id");
        return;
    }
    res.status(200);
    res.json({
        potenID,
    });
});

app.post("/removePotenID", (req, res) => {
    const id = req.body.potenID;
    const success = potentialQueue.remove(id);

    if (!success) {
        res.status(500);
        console.log("Something went wrong removing from potential queue.");
        return;
    }

    res.status(200);
});

app.post("/getQueueSpotInfo", (req, res) => {
    const id = req.body.id;
    const index = queue.customerIndexOnQueue(id);
    if (index == -1) {
        res.status(404);
        console.log(`ID: ${id} not in queue. Could not get position.`);
        res.json({ success: false });
        return;
    }
    const data = { queuePos: index + 1 };
    res.status(200);
    if (index === 0) {
        if (queue.firstCustomerCanEnter()) {
            data.canEnter = true;
            data.checkInTimer = queue.getCheckInTimer();
        } else {
            data.canEnter = false;
        }
    }
    res.json({ success: true, data: data });
});

app.post("/leaveQueue", (req, res) => {
    const id = req.body.id;
    const removedCustomer = queue.deleteCustomerWithID(id);
    if (removedCustomer == undefined) {
        res.status(404);
        console.log(`ID: ${id} not in queue. Could not remove.`);
        res.json({ success: false });
        return;
    }

    res.status(200);
    res.json({ success: true });

    const socketIDs = queue.getSocketIDs();
    serverSocket.alertChange(io, socketIDs);
});

universalEmitter.on("updateFlag", updateFlagHandler);
function updateFlagHandler() {
    const socketIDs = queue.getSocketIDs();
    serverSocket.alertChange(io, socketIDs);
}

universalEmitter.on("startedCheckIn", function () {
    var subObj = queue.getSubcriptionObj(0);
    const payload = JSON.stringify({
        title: `It is your turn to enter the store.`,
    });
    notification.triggerPushMsg(subObj, payload);

    var socketID = queue.getSocketID(0);
    serverSocket.startedCheckIn(io, socketID);
});

// ----------------------------------------------------x
// ------------------MCU REQUESTS----------------------x
app.get("/barcodeScan", (req, res) => {
    const storeSecret = req.query.storeSecret;
    if (storeSecret.localeCompare(STORE_SECRET)) {
        console.log("BAD SECRET");
        res.status(401);
        res.json(store.errorResponse());
        return;
    }
    const id = req.query.IDscanned;
    var pos = queue.customerIndexOnQueue(id);
    if (pos !== -1) {
        var queueSpot = queue.get(pos);
        var customer = queueSpot.customer;
        if (pos === 0) {
            var checkedInQueueSpot = queue.checkInCustomer();
            res.status(200);
            res.json(store.enteringResponse(customer));

            console.log("Customer entering the store", checkedInQueueSpot);
            console.log("Check in has started");
            serverSocket.checkedIn(io, checkedInQueueSpot.socketID);
        } else {
            console.log(`Customer is trying entering the store.`, customer);
            res.status(200);
            res.json(store.earlyEnterResponse(customer));
            // add json for barcode scan going back to the MCU
        }
    } else {
        const potenID = id;
        if (!potentialQueue.exists(potenID)) {
            console.log("BAD ID");
            res.status(200);
            res.json(store.errorResponse());
            return;
        }

        const { potenCustomer, socketID } = potentialQueue.get(potenID);
        potentialQueue.remove(potenID);

        // add id to the customer since now they have joined the queue
        const customer = customerHelper.make(potenCustomer, potenID);
        if (
            circumvent &&
            queue.length() == 0 &&
            store.isAllowToEnterStore(customer.numPeople)
        ) {
            console.log(
                `Customer is being let into the store without joining the queue.`,
                customer
            );
            console.log("Check in has started");
            res.status(200);
            res.json(store.enteringResponse(customer));

            serverSocket.checkedIn(io, socketID);
            return;
        }

        queue.addCustomerToQueue(customer);
        console.log(`Customer has joined the queue.`, customer);

        serverSocket.moveToQueue(io, socketID, customer);
        res.status(200);
        res.json(store.joinQueueResponse());
    }

    // http://localhost:3001/barcodeScan?storeSecret=grp4&IDscanned=
});

app.get("/leftStore", (req, res) => {
    const storeSecret = req.query.storeSecret;
    if (storeSecret.localeCompare(STORE_SECRET)) {
        console.log("BAD SECRET");
        res.status(401);
        res.json({ ok: false });
        return;
    }
    res.status(200);
    res.json({ ok: true });
    console.log(`Someone left the store.`);

    store.leftStore();
});

app.get("/enteredStore", (req, res) => {
    const storeSecret = req.query.storeSecret;
    if (storeSecret.localeCompare(STORE_SECRET)) {
        console.log("BAD SECRET");
        res.status(401);
        res.json({ ok: false });
        return;
    }

    res.status(200);
    res.json({ ok: true });
    console.log(`Someone entered the store.`);

    store.enteredStore();
});

app.get("/tempError", (req, res) => {
    const storeSecret = req.query.storeSecret;
    const temp = req.query.temp;
    if (storeSecret.localeCompare(STORE_SECRET)) {
        console.log("BAD SECRET");
        res.status(401);
        res.json({ ok: false });
        return;
    }

    errorMgmt.putTemp(temp);
    console.log(`Someone had a wrong temp.`);
    res.status(200);
    res.json({ ok: true });
});

app.get("/unauthorizedEntry", (req, res) => {
    const storeSecret = req.query.storeSecret;
    if (storeSecret.localeCompare(STORE_SECRET)) {
        console.log("BAD SECRET");
        res.status(401);
        res.json({ ok: false });
        return;
    }

    errorMgmt.putEntry();
    console.log(`Someone entered without authorization.`);

    res.status(200);
    res.json({ ok: true });
});

app.get("/getStatus", (req, res) => {
    const storeSecret = req.query.storeSecret;
    if (storeSecret.localeCompare(STORE_SECRET)) {
        console.log("BAD SECRET");
        res.status(401);
        res.json({ ok: false });
        return;
    }

    res.status(200);
    res.json({
        ok: true,
        queueLength: queue.length(),
        numPeopleInStore: store.getNumPeople(),
        maxCapacity: store.getMaxCapacity(),
    });
});

app.get("/checkInDone", (req, res) => {
    const storeSecret = req.query.storeSecret;
    if (storeSecret.localeCompare(STORE_SECRET)) {
        console.log("BAD SECRET");
        res.status(401);
        res.json({ ok: false });
        return;
    }
    console.log(`Check in finished`);

    queue.setIDLE();

    res.status(200);
    res.json({ ok: true });
});

// ----------------------------------------------------x
// ------------------CONSOLE REQUESTS------------------x
var SESSION_KEY = undefined;
var CONSOLE_SOCKET_ID = undefined;

app.post("/consoleLogin", (req, res) => {
    const data = req.body;
    const storeSecret = data.storeSecret;

    if (storeSecret != STORE_SECRET) {
        res.status(401);
        res.json({});
        return;
    }

    //assign SESSION_KEY
    SESSION_KEY = _genHexString(16);

    res.status(200);
    res.json({
        success: true,
        sessionKey: SESSION_KEY,
    });
});

app.post("/verifySessionKey", (req, res) => {
    const data = req.body;
    const sessionKey = data.sessionKey;

    if (!SESSION_KEY || sessionKey != SESSION_KEY) {
        res.status(401);
        res.json({});
        return;
    }

    res.status(200);
    res.json({});
});

app.post("/getStatusConsole", (req, res) => {
    const data = req.body;
    const sessionKey = data.sessionKey;

    if (!SESSION_KEY || sessionKey != SESSION_KEY) {
        res.status(401);
        res.json({});
        return;
    }

    res.status(200);
    res.json({
        maxCapacity: store.getMaxCapacity(),
        numPeople: store.getNumPeople(),
        queue: queue.getFullQueue(),
        unauthorizedEntries: errorMgmt.getFullEntry(),
        tempErrors: errorMgmt.getFullTemp(),
    });
});

app.post("/removeTempError", (req, res) => {
    const data = req.body;
    const sessionKey = data.sessionKey;

    if (!SESSION_KEY || sessionKey != SESSION_KEY) {
        res.status(401);
        res.json({});
        return;
    }

    const index = data.removeIndex;
    errorMgmt.removeTemp(index);

    res.status(200);
    res.json({
        success: true,
    });
});

app.post("/removeUnauthorizedEntry", (req, res) => {
    const data = req.body;
    const sessionKey = data.sessionKey;

    if (!SESSION_KEY || sessionKey != SESSION_KEY) {
        res.status(401);
        res.json({});
        return;
    }

    const index = data.removeIndex;
    errorMgmt.removeEntry(index);

    res.status(200);
    res.json({
        success: true,
    });
});

app.post("/updateMaxCapacity", (req, res) => {
    const data = req.body;
    const sessionKey = data.sessionKey;

    if (!SESSION_KEY || sessionKey != SESSION_KEY) {
        res.status(401);
        res.json({});
        return;
    }

    const newMaxCapacity = data.newMaxCapacity;
    store.setStoreCapacity(newMaxCapacity);

    res.status(200);
    res.json({
        success: true,
    });
});

app.post("/updateNumPeople", (req, res) => {
    const data = req.body;
    const sessionKey = data.sessionKey;

    if (!SESSION_KEY || sessionKey != SESSION_KEY) {
        res.status(401);
        res.json({});
        return;
    }

    const newNumPeople = data.newNumPeople;
    store.setNumPeople(newNumPeople);

    res.status(200);
    res.json({
        success: true,
    });
});

universalEmitter.on("consoleConnection", function (socketID) {
    CONSOLE_SOCKET_ID = socketID;
});

universalEmitter.on("statusChange", function () {
    serverSocket.consoleUpdate(io, CONSOLE_SOCKET_ID);
});

function _genHexString(len) {
    const hex = "0123456789ABCDEF";
    let output = "";
    for (let i = 0; i < len; ++i) {
        output += hex.charAt(Math.floor(Math.random() * hex.length));
    }
    return output;
}
// ----------------------------------------------------x

httpServer.listen(PORT, () => console.log(`Http listening at port ${PORT}`));
