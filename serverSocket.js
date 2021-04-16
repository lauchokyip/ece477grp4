const { Socket } = require("socket.io");
const potentialQueue = require("./potentialQueue.js");
const queue = require("./queue.js");
const universalEmitter = require("./universalEmitter.js");

function connection(socket) {
    console.log("socket connected: " + socket.id);

    socket.on("potenID", function (data) {
        const potenID = data.potenID;

        potentialQueue.setSocketID(potenID, socket.id);

        // console.log(
        //     "Socket ID added to potential queue entry",
        //     potentialQueue.get(potenID)
        // );
    });

    socket.on("id", function (data) {
        const id = data.id;

        const index = queue.customerIndexOnQueue(id);
        queue.addSocketIDToIndex(index, socket.id);
    });

    socket.on("consoleConnection", function () {
        universalEmitter.emit("consoleConnection", socket.id);
    });
}

function moveToQueue(io, socketID, customer) {
    // console.log(io);
    const data = {
        customer,
    };
    console.log("sending user with socket ID to the queue", socketID);
    io.to(socketID).emit("moveToQueue", data);
}

function startedCheckIn(io, socketID) {
    io.to(socketID).emit("startedCheckIn");
}

function checkedIn(io, socketID) {
    console.log("Checked in socket ID", socketID);
    io.to(socketID).emit("checkedIn");
}

function alertChange(io, socketIDs) {
    socketIDs.forEach((socketID) => {
        io.to(socketID).emit("queueChange");
    });
}

function consoleUpdate(io, socketID) {
    console.log(`sending consoleUpdate to ${socketID}`);
    io.to(socketID).emit("consoleUpdate");
}

module.exports = {
    connection,
    moveToQueue,
    alertChange,
    startedCheckIn,
    checkedIn,
    consoleUpdate,
};
