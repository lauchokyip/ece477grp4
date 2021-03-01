// Package level import
const express = require("express");
const http = require("http");
const socket = require("socket.io");
const webpush = require("web-push");
const bodyParser = require("body-parser");

// User-defined import
const potentialQueue = require("./potentialQueue.js");
const serverSocket = require("./serverSocket.js");
const queue = require("./queue.js");
const notification = require("./notification");
const customerHelper = require("./helper/customerHelper.js");

const PORT = process.env.PORT || 3001;
const app = express();
const httpServer = http.Server(app);
const io = socket(httpServer);
const STORE_SECRET = "grp4";

// middleware
app.use(express.static("public"));
app.use("/helper", express.static("helper"));
app.use("/bootstrap", express.static("node_modules/bootstrap/dist/css"));
app.use(express.json({ limit: "1mb" }));
app.use(bodyParser.json());
app.set("view engine", "ejs");

io.sockets.on("connection", serverSocket.connection);
// TODO: Need an API to set up the store capacity

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

// this will remove clear all the array of Subscription Object
app.get("/deleteSub", (req, res) => {
    notification.clearAllSubscriptionsFromDatabaes().then((subscriptions) => {
        res.status(200);
        res.setHeader("Content-Type", "application/json");
        res.send(JSON.stringify({ data: subscriptions }));
    });
});

// test admin push Route
app.get("/adminTestPush", (req, res) => {
    notification
        .getSubscriptionsFromDatabase()
        .then((subscriptions) => {
            const payload = JSON.stringify({
                title: "Broadcasting messages to everyone",
            });
            let promiseChain = Promise.resolve();
            for (let i = 0; i < subscriptions.length; i++) {
                const subscription = subscriptions[i]["subscription"];
                promiseChain = promiseChain.then(() => {
                    return notification.triggerPushMsg(subscription, payload);
                });
            }
            res.setHeader("Content-Type", "application/json");
            res.send(JSON.stringify({ data: subscriptions }));
        })
        .catch(function (err) {
            res.status(500);
            res.setHeader("Content-Type", "application/json");
            res.send(
                JSON.stringify({
                    error: {
                        id: "unable-to-send-messages",
                        message:
                            `We were unable to send messages to all subscriptions : ` +
                            `'${err.message}'`,
                    },
                })
            );
        });
});

// test pushing to single user
app.get("/testPush", (req, res) => {
    const customerId = req.query.id
    console.log(customerId)
    notification.getSubscriptionsByID(customerId)
    .then( (subscriptionObj) => {
        const payload = JSON.stringify({
            title: "It's your turn",
        });
        notification.triggerPushMsg(subscriptionObj, payload)
        res.setHeader("Content-Type", "application/json");
        res.send(JSON.stringify({ message: `Data successfully sent to '${customerId}'`  }));
    })
    .catch((err) => {
        res.status(500);
        res.setHeader("Content-Type", "application/json");
        res.send(
            JSON.stringify({
                error: {
                    id: "unable-to-send-messages",
                    message:
                        `We were unable to send messages to all subscriptions : ` +
                        `'${err}'`,
                },
            })
        );
    })
})

// ----------------------------------------------------x
// -----------------POST REQUESTS----------------------x
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

// Subscribe Route
app.post("/subscribe", (req, res) => {
    if (!notification.isValidSaveRequest(req, res)) {
        return;
    }
    notification
        .saveSubscriptionToDatabase(req.body)
        .then(function (subscriptionId) {
            res.setHeader("Content-Type", "application/json");
            res.send(JSON.stringify({ data: { success: true } }));
        })
        .catch(function (err) {
            res.status(500);
            res.setHeader("Content-Type", "application/json");
            res.send(
                JSON.stringify({
                    error: {
                        id: "unable-to-save-subscription",
                        message:
                            "The subscription was received but we were unable to save it to our database.",
                    },
                })
            );
        });
});



// ----------------------------------------------------x
// ------------------MCU REQUESTS----------------------x
app.get("/enterQueue", (req, res) => {
    const storeSecret = req.query.storeSecret;
    if (storeSecret.localeCompare(STORE_SECRET)) {
        console.log("BAD SECRET");
        res.status(401);
        res.json({ success: false });
        return;
    }

    const potenID = req.query.potenID;
    if (!potentialQueue.exists(potenID)) {
        console.log("BAD POTENTIAL ID");
        res.status(404);
        res.json({ success: false });
        return;
    }

    const { potenCustomer, socketID } = potentialQueue.get(potenID);
    potentialQueue.remove(potenID);

    // add id to the customer since now they have joined the queue
    const customer = customerHelper.make(potenCustomer, potenID);
    queue.addCustomerToQueue(customer);

    serverSocket.moveToQueue(io, socketID, customer);
    res.status(200);
    res.json({ success: true });

    // http://localhost:3001/enterQueue?storeSecret=grp4&potenID=
});
// ----------------------------------------------------x

httpServer.listen(PORT, () => console.log(`Http listening at port ${PORT}`));