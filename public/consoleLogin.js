const txtStoreSecret = document.getElementById("txtStoreSecret");
const btnLogin = document.getElementById("btnLogin");
const errorText = document.getElementById("errorText");

btnLogin.addEventListener("click", async function () {
    errorText.innerHTML = "";
    var storeSecret = txtStoreSecret.value;

    const data = {
        storeSecret,
    };
    const options = {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    };

    const response = await fetch("/consoleLogin", options);
    if (response.status != 200) {
        errorText.innerHTML = "Wrong store secret";
        return;
    } else {
        const responseData = await response.json();
        localStorage.setItem("sessionKey", responseData.sessionKey);
        window.location.href = "/console";
    }
});
