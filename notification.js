const webpush = require("web-push");
const publicVapidKey =
    "BKUIR2K8S-WoOEqPaluuHgRLf7_ud8OxSMVGiOzUrtBzWdM7mOhDRkwsty5dJsyYcWZtSiVjpsOE1WZofG3v-S0";
const privateVapidKey = "qW6a1325o4JjBqUc7ylFLeL88Y_4Fgp28sUBCNLOYZI";

// let db = [];

webpush.setVapidDetails(
    "mailto:test@test.com",
    publicVapidKey,
    privateVapidKey
);

/**
 * triggerPushMsg will push message once it's called
 * @param {*} subscription
 * @param {*} dataToSend
 */
function triggerPushMsg(subscription, dataToSend) {
    if (subscription === undefined || subscription === null) {
        return;
    }
    return webpush.sendNotification(subscription, dataToSend);
}

function triggerPushMsgArr(subObjArr, dataToSend) {
    subObjArr.forEach((subObj) => {
        triggerPushMsg(subObj, dataToSend);
    });
}
/**
 * isValidSaveRequest will validate if the request is valid or not
 * @param {} req
 * @param {*} res
 */
const isValidSaveRequest = (req, res) => {
    // Check the request body has at least an endpoint.
    if (!req.body || !req.body.subscription.endpoint) {
        // Not a valid subscription.
        res.status(400);
        res.setHeader("Content-Type", "application/json");
        res.send(
            JSON.stringify({
                error: {
                    id: "no-endpoint",
                    message: "Subscription must have an endpoint.",
                },
            })
        );
        return false;
    }
    return true;
};

module.exports = {
    isValidSaveRequest,
    triggerPushMsg,
    triggerPushMsgArr,
};
