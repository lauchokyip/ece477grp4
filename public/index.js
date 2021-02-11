const txtName = document.getElementById("txtName");
const txtEmail = document.getElementById("txtEmail");
const txtNumPeople = document.getElementById("txtNumPeople");
const btnJoinQueue = document.getElementById("btnJoinQueue");

txtName.addEventListener("change", validateInput);
txtEmail.addEventListener("change", validateInput);
txtNumPeople.addEventListener("change", validateInput);

function validateInput() {
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

btnJoinQueue.addEventListener("click", function () {});
