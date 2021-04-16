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
const slctNumPeople = document.getElementById("slctNumPeople");
const btnJoinQueue = document.getElementById("btnJoinQueue");

txtName.addEventListener("input", validateInput);
txtEmail.addEventListener("input", validateInput);
slctNumPeople.addEventListener("input", validateInput);

var btnClicked = false;

init();
function init() {
    for (let i = 1; i <= customerHelper.MAX_PEOPLE_IN_PARTY; i++) {
        var opt = document.createElement("option");
        opt.value = i;
        opt.innerHTML = i;
        slctNumPeople.append(opt);
    }
}

function validateInput() {
    if (btnClicked) {
        return;
    }

    console.log("running validate");

    const potenCustomer = customerHelper.makePoten(
        txtName.value,
        txtEmail.value,
        slctNumPeople.value
    );

    if (potenCustomer === null) {
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

    const potenCustomer = customerHelper.makePoten(
        txtName.value,
        txtEmail.value,
        slctNumPeople.value
    );

    var potenID = await getPotenID(potenCustomer);
    if (potenID !== null) {
        localStorage.setItem("potenCustomer", JSON.stringify(potenCustomer));
        window.location.href = "/joinQueue";
    } else {
        alert("Something went wrong generating customer ID");
        btnClicked = false;
        enableBtn();
    }
});
