// TODO: force people to use Chrome
var isChromium = window.chrome;
var winNav = window.navigator;
var vendorName = winNav.vendor;
var isOpera = typeof window.opr !== "undefined";
var isIEedge = winNav.userAgent.indexOf("Edge") > -1;
var isIOSChrome = winNav.userAgent.match("CriOS");

if (isIOSChrome) {
    // is Google Chrome on IOS
    console.log("Is Google Chrome");
} else if (
    isChromium !== null &&
    typeof isChromium !== "undefined" &&
    vendorName === "Google Inc." &&
    isOpera === false &&
    isIEedge === false
) {
    // is Google Chrome
    console.log("Is Google Chrome");
} else {
    // not Google Chrome
    // TODO: tell user to use Google Chrome
    console.log("Is Not Google Chrome");
}

// Ask for permission (must)
if (window.Notification) {
    Notification.requestPermission((status) => {
        console.log("Status of the request:", status);
        if (Notification.permission !== "granted") {
            // TODO: Display that we need notification in order for it to work
        }
    });
}

// Check if push messaging is supported
if (!("PushManager" in window)) {
    console.warn("Push messaging isn't supported.");
}

const txtName = document.getElementById("txtName");
const txtEmail = document.getElementById("txtEmail");
const txtNumPeople = document.getElementById("txtNumPeople");
const btnJoinQueue = document.getElementById("btnJoinQueue");

txtName.addEventListener("input", validateInput);
txtEmail.addEventListener("input", validateInput);
txtNumPeople.addEventListener("input", validateInput);

var btnClicked = false;

function validateInput() {
    if (btnClicked) {
        return;
    }

    console.log("running validate");
    if (txtName.value.length === 0) {
        disableBtn();
        return;
    }
    if (txtEmail.value.length === 0) {
        disableBtn();
        return;
    }
    if (txtNumPeople.value.length === 0) {
        disableBtn();
        return;
    }
    var numPeople;
    try {
        numPeople = parseInt(txtNumPeople.value);
    } catch (err) {
        disableBtn();
        return;
    }
    if (numPeople <= 0) {
        disableBtn();
        return;
    }

    enableBtn();
}

function disableBtn() {
    btnJoinQueue.disabled = true;
}
function enableBtn() {
    btnJoinQueue.disabled = false;
}

btnJoinQueue.addEventListener("click", async function () {
    btnClicked = true;
    disableBtn();

    const customer = {
        name: txtName.value,
        email: txtEmail.value,
        numPeopleInParty: parseInt(txtNumPeople.value),
    };

    var potenID = await getPotenID(customer);
    if (potenID !== null) {
        localStorage.setItem("potenCustomer", JSON.stringify(customer));
        window.location.href = "/joinQueue";
    } else {
        alert("Something went wrong generating customer ID");
        btnClicked = false;
        enableBtn();
    }
});
