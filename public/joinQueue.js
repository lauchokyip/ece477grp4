const divQRCode = document.getElementById("qrcode");
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

    qrcode = makeQRCode(potenID, divQRCode);
    destroyQRTimeout = setTimeout(function () {
        removeQRCode();
    }, remainingTTL(potenID));

    linkSocketToID(potenID.id);
}

btnNewQRCode.addEventListener("click", async function () {
    removeQRCode();
    await generateNewID();
    displayQRCode();
});

function removeQRCode() {
    // console.log("ID expired removing QR code.");
    for (let i = 0; i < divQRCode.children.length; i++) {
        const element = divQRCode.children[i];
        element.remove();
    }
}

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

function makeQRCode(potenId, div) {
    var QRContainer = document.createElement("div");
    div.append(QRContainer);
    var qrcode = new QRCode(QRContainer, {
        text: potenId.id,
        width: 128,
        height: 128,
        colorDark: "#000000",
        colorLight: "#ffffff",
        correctLevel: QRCode.CorrectLevel.H,
    });

    return qrcode;
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
});

function linkSocketToID(potenID) {
    data = {
        potenID,
    };
    socket.emit("potenID", data);
}
