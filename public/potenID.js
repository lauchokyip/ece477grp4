async function getPotenID(potenCustomer) {
    const data = {
        potenCustomer,
    };
    const options = {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    };

    const response = await fetch("/getPotenID", options);
    if (response.status === 200) {
        const responseData = await response.json();
        console.log(responseData);

        const { potenID } = responseData;
        localStorage.setItem("potenID", JSON.stringify(potenID));
        return potenID;
    } else {
        return null;
    }
}

async function removePotenID(id) {
    const data = {
        potenID: id,
    };
    const options = {
        method: "POST",
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
    };

    const response = await fetch("/removePotenID", options);
    if (response.status === 200) {
        console.log("Removed poten id from server");
    } else {
        console.log("Error occured while removing poten id from server");
    }
}
