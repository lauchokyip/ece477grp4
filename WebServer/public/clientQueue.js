// import { registerSW } from "./registerServiceWorker.js";

const positionText = document.getElementById("positionText");
const timerText = document.getElementById("timerText");
const btnLeaveQueue = document.getElementById("btnLeaveQueue");

var socket = io();
var clkInterval = null;
var expiredTimeout = null;

init();
function init() {
    const customer = JSON.parse(localStorage.getItem("customer"));
    const id = customer.id;
    registerSW(id).catch((error) => {
        console.error(error);
        alert("error registering service worker");
    });
}

async function requestInfo() {
    const customer = JSON.parse(localStorage.getItem("customer"));
    const id = customer.id;
    const data = {
        id,
    };
    const options = {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    };

    const response = await fetch("/getQueueSpotInfo", options);
    if (response.status != 200) {
        return -1;
    }
    const responseData = await response.json();
    console.log(responseData);
    const queuePos = responseData.data.queuePos;
    const canEnter = responseData.data.canEnter;
    const checkInTimer = responseData.data.checkInTimer;

    displayPos(queuePos, canEnter, checkInTimer);

    return queuePos;
}

function displayPos(queuePos, canEnter, checkInTimer) {
    if (queuePos === 1 && canEnter) {
        const customer = JSON.parse(localStorage.getItem("customer"));
        const id = customer.id;
        makeQRCode(id);

        var checkInTimer = checkInTimer;
        var timeToEnter = checkInTimer.timeToEnter;

        clkInterval = setInterval(function () {
            var timeLeft = (timeToEnter - Date.now()) / 1000;
            // console.log(`TIME LEFT TO CHECK IN: ${timeLeft} seconds`);
            timerText.innerHTML = timeLeft;
        }, 1000);
        expiredTimeout = setTimeout(function () {
            clearInterval(clkInterval);
            alert("Time to check in expired.");
            location.href = "/";
            return;
        }, timeToEnter - Date.now());
    } else {
        removeQRCode();
        timerText.innerHTML = "";
        clearInterval(clkInterval);
        clearTimeout(expiredTimeout);
    }
    positionText.innerHTML = queuePos;
}

function checkCustomerInfo() {
    const customer = JSON.parse(localStorage.getItem("customer"));
    return customerHelper.isInvalid(customer);
}

socket.on("connect", async function () {
    var err = checkCustomerInfo();
    if (err) {
        alert("Wrong customer information");
        location.href = "/";
        return;
    }

    var pos = await requestInfo();
    if (pos == -1) {
        alert("Customer not in queue. Cannot get position.");
        location.href = "/";
        return;
    }

    const customer = JSON.parse(localStorage.getItem("customer"));
    const id = customer.id;

    let data = {
        id,
    };
    socket.emit("id", data);
});

socket.on("queueChange", updatePos);
socket.on("startedCheckIn", updatePos);

async function updatePos() {
    var pos = await requestInfo();
    if (pos == -1) {
        alert("Customer not in queue. Cannot update position.");
        location.href = "/";
    }
}

socket.on("checkedIn", function () {
    console.log("recieved checked in");
    window.location.href = "/endScreen";
});

btnLeaveQueue.addEventListener("click", async function () {
    const customer = JSON.parse(localStorage.getItem("customer"));
    const id = customer.id;
    const data = {
        id,
    };
    const options = {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    };

    const response = await fetch("/leaveQueue", options);
    if (response.status != 200) {
        alert("Already not in queue.");
    }

    window.location.href = "/";
});
