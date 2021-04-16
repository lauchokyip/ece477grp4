const divQRCode = document.getElementById("qrcode");

function makeQRCode(id) {
    removeQRCode();
    var QRContainer = document.createElement("div");
    divQRCode.append(QRContainer);
    var qrcode = new QRCode(QRContainer, {
        text: id,
        width: 128,
        height: 128,
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
