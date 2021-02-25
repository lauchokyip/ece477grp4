const potentialQueue = require("./potentialQueue.js");

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
}

function moveToQueue(io, socketID, customer) {
    // console.log(io);
    const data = {
        customer,
    };
    console.log("sending user with socket ID to the queue", socketID);
    io.to(socketID).emit("moveToQueue", data);
}

module.exports = { connection, moveToQueue };
