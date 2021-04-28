const divQRCode = document.getElementById("qrcode");
const MAX_QR_WIDTH = 500;
const QR_MARGIN = 20;

function makeQRCode(id) {
    removeQRCode();

    var w = window.innerWidth;

    if (w - QR_MARGIN * 2 <= MAX_QR_WIDTH) {
        w = w - QR_MARGIN * 2;
    } else {
        w = MAX_QR_WIDTH;
    }
    var QRContainer = document.createElement("div");
    QRContainer.classList += "center ";
    QRContainer.style.width = w + "px";
    divQRCode.append(QRContainer);
    var qrcode = new QRCode(QRContainer, {
        text: id,
        width: w,
        height: w,
        colorDark: "#000000",
        colorLight: "#ffffff",
        correctLevel: QRCode.CorrectLevel.H,
    });
    console.log(id);
    return qrcode;
}

function removeQRCode() {
    // console.log("ID expired removing QR code.");
    for (let i = 0; i < divQRCode.children.length; i++) {
        const element = divQRCode.children[i];
        element.remove();
    }
}
