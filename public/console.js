var sessionKey = undefined;
var systemStatus = {};
var socket = io();

const maxCapacityText = document.getElementById("maxCapacityText");
const numPeopleText = document.getElementById("numPeopleText");
const queueLenText = document.getElementById("queueLenText");

const txtNewMaxCapacity = document.getElementById("txtNewMaxCapacity");
const btnSetMaxCapacity = document.getElementById("btnSetMaxCapacity");
const setMaxCapacityErrorText = document.getElementById(
    "setMaxCapacityErrorText"
);

const txtNewNumPeople = document.getElementById("txtNewNumPeople");
const btnSetNumPeople = document.getElementById("btnSetNumPeople");
const setNumPeopleErrorText = document.getElementById("setNumPeopleErrorText");

btnSetMaxCapacity.addEventListener("click", async function () {
    setMaxCapacityErrorText.innerHTML = "";
    var val = txtNewMaxCapacity.value;
    if (!isPositiveInt(val)) {
        setMaxCapacityErrorText.innerHTML =
            "Max Capacity should be a positive integer or zero";
        txtNewMaxCapacity.value = "";
        return;
    }

    const data = {
        sessionKey,
        newMaxCapacity: parseInt(val),
    };
    console.log(data);
    const options = {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    };

    const response = await fetch("/updateMaxCapacity", options);
    if (response.status != 200) {
        alert("Unauthorized access.");
        window.location.href = "/updateMaxCapacity";
        return;
    }

    txtNewMaxCapacity.value = "";
});

btnSetNumPeople.addEventListener("click", async function () {
    setNumPeopleErrorText.innerHTML = "";
    var val = txtNewNumPeople.value;
    if (!isPositiveInt(val)) {
        setNumPeopleErrorText.innerHTML =
            "Number of people in store should be a positive integer or zero";
        txtNewNumPeople.value = "";
        return;
    }

    const data = {
        sessionKey,
        newNumPeople: parseInt(val),
    };
    console.log(data);
    const options = {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    };

    const response = await fetch("/updateNumPeople", options);
    if (response.status != 200) {
        alert("Unauthorized access.");
        window.location.href = "/consoleLogin";
        return;
    }

    txtNewNumPeople.value = "";
});

function isPositiveInt(num) {
    if (isNaN(parseInt(num))) return false;
    if (num % 1 !== 0) return false;
    if (num < 0) return false;
    return true;
}

// init();

socket.on("connect", init);

async function init() {
    sessionKey = localStorage.getItem("sessionKey");
    var verified = await verify();
    if (!verified) {
        window.location.href = "/consoleLogin";
        return;
    }
    socket.emit("consoleConnection");
    update();
}

async function getStatus() {
    const data = {
        sessionKey,
    };
    const options = {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    };

    const response = await fetch("/getStatusConsole", options);
    if (response.status != 200) {
        alert("Unauthorized access.");
        window.location.href = "/consoleLogin";
        return;
    }
    const responseData = await response.json();
    systemStatus = responseData;
}

function displayStatus() {
    console.log(systemStatus);
    maxCapacityText.innerHTML = systemStatus.maxCapacity;
    numPeopleText.innerHTML = systemStatus.numPeople;
    queueLenText.innerHTML = systemStatus.queue.length;
}

socket.on("consoleUpdate", function () {
    console.log("Console Update");
    update();
});

async function update() {
    await getStatus();
    displayStatus();
}

async function verify() {
    const data = {
        sessionKey,
    };
    const options = {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    };

    const response = await fetch("/verifySessionKey", options);
    if (response.status != 200) {
        alert("Wrong key.");
        return false;
    }
    return true;
}
