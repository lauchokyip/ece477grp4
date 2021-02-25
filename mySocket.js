function connection(socket) {
    console.log("socket connected: " + socket.id);
    socket.emit("trialReq");
}

module.exports = { connection };
