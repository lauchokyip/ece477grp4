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

const tblUnauthorizedEntries = document.getElementById(
    "tblUnauthorizedEntries"
);
const tblTempErrors = document.getElementById("tblTempErrors");

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
    displayEntryTable(systemStatus.unauthorizedEntries);
    displayTempTable(systemStatus.tempErrors);
}

function displayEntryTable(unauthorizedEntries) {
    var keyArr = ["time"];
    unauthorizedEntries.forEach((elem) => {
        elem.time = _getTimeStr(elem.timestamp);
    });

    async function removeFunc() {
        var index = this.index;
        const data = {
            sessionKey,
            removeIndex: index,
        };
        const options = {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
            },
            body: JSON.stringify(data),
        };

        const response = await fetch("/removeUnauthorizedEntry", options);
        if (response.status != 200) {
            alert("Unauthorized access.");
            window.location.href = "/consoleLogin";
            return;
        }
    }

    displayTable(
        tblUnauthorizedEntries,
        keyArr,
        unauthorizedEntries,
        removeFunc
    );
}

function displayTempTable(tempErrors) {
    var keyArr = ["temp", "time"];
    tempErrors.forEach((elem) => {
        elem.time = _getTimeStr(elem.timestamp);
    });

    async function removeFunc() {
        var index = this.index;
        const data = {
            sessionKey,
            removeIndex: index,
        };
        const options = {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
            },
            body: JSON.stringify(data),
        };

        const response = await fetch("/removeTempError", options);
        if (response.status != 200) {
            alert("Unauthorized access.");
            window.location.href = "/consoleLogin";
            return;
        }
    }

    displayTable(tblTempErrors, keyArr, tempErrors, removeFunc);
}

function displayTable(tblElem, keyArr, objArr, removeFunc) {
    while (tblElem.children.length !== 1) {
        tblElem.deleteRow(1);
    }
    var i = objArr.length + 1;
    var index = 0;
    objArr.forEach((obj) => {
        i -= 1;
        var row = document.createElement("tr");

        var cell = document.createElement("td");
        cell.innerText = i;
        row.append(cell);

        keyArr.forEach((key) => {
            var cell = document.createElement("td");
            cell.innerText = obj[key];
            row.append(cell);
        });

        var cell = document.createElement("td");
        var btn = document.createElement("input");
        btn.type = "button";
        btn.value = "Dismiss";
        // btn.classList = "dismiss ";
        btn.index = index;
        btn.addEventListener("click", removeFunc);
        cell.append(btn);
        row.append(cell);

        tblElem.append(row);
        index += 1;
    });
}

function _getTimeStr(timestamp) {
    var ts = new Date(timestamp);
    var str = ts.toLocaleTimeString();
    // var patt = /[0-9]+:[0-9]+(:[0-9]+) /i;
    // var result = str.match(patt);
    // str = str.replace(result[1], "");

    return str;
}

async function update() {
    await getStatus();
    displayStatus();
}

socket.on("consoleUpdate", function () {
    console.log("Console Update");
    update();
});

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
