// import { registerSW } from "./registerServiceWorker.js";
// const divQRCode = document.getElementById("qrcode");
const btnNewQRCode = document.getElementById("btnNewQRCode");
var destroyQRTimeout;
var qrcode;

var socket = io();

displayQRCode();

async function displayQRCode() {
    const potenID = JSON.parse(localStorage.getItem("potenID"));
    if (potenID === null || !checkTTL(potenID)) {
        alert("No ID or ID expired. Generating new ID");
        var success = await generateNewID();
        if (!success) {
            return;
        }
        displayQRCode();
        return;
    }

    qrcode = makeQRCode(potenID.id);
    destroyQRTimeout = setTimeout(function () {
        removeQRCode();
    }, remainingTTL(potenID));

    linkSocketToID(potenID.id);

    // for debugging
    document.getElementById("qrcodeText").innerHTML = potenID.id;
}

btnNewQRCode.addEventListener("click", async function () {
    removeQRCode();
    await generateNewID();
    displayQRCode();
});

async function generateNewID() {
    localStorage.removeItem("potenID");
    clearTimeout(destroyQRTimeout);
    var potenCustomer = JSON.parse(localStorage.getItem("potenCustomer"));
    if (potenCustomer === null) {
        alert("leaving page because getting no customer data");
        window.location.replace("/");
        return false;
    }
    var potenIDnew = await getPotenID(potenCustomer);
    if (potenIDnew === null) {
        alert("leaving page because getting new poten id failed");
        window.location.replace("/");
        return false;
    }
    localStorage.setItem("potenID", JSON.stringify(potenIDnew));

    return true;
}

function checkTTL(potenID) {
    return Date.now() < potenID.timestamp + potenID.ttl;
}

function remainingTTL(potenID) {
    return Date.now() - potenID.timestamp + potenID.ttl;
}

socket.on("connect", function () {
    console.log("Socket connection");
    const potenID = JSON.parse(localStorage.getItem("potenID"));
    if (potenID === null || !checkTTL(potenID)) {
        return;
    }
    linkSocketToID(potenID);
});

socket.on("moveToQueue", function (data) {
    localStorage.setItem("customer", JSON.stringify(data.customer));
    localStorage.removeItem("potenID");
    localStorage.removeItem("potenCustomer");
    window.location.href = "/queue";

    // subscribe over here after the customer
});

socket.on("checkedIn", function () {
    console.log("recieved checked in");
    window.location.href = "/endScreen";
});

function linkSocketToID(potenID) {
    let data = {
        potenID,
    };
    socket.emit("potenID", data);
}
