const crypto = require("crypto");

const ID_TTL = 1800 * 1000;
const ID_BITLEN = 32;

const potentialQueue = {};

function generateID(potenCustomer) {
    if (potenCustomer === null) {
        return null;
    }

    var id;
    while (true) {
        var timestamp = Date.now();
        id = crypto
            .createHash("sha1")
            .update(potenCustomer.name + timestamp)
            .digest("hex")
            .substring(0, ID_BITLEN / 4);

        if (potentialQueue[id] === undefined) {
            break;
        }
    }

    potentialQueue[id] = { potenCustomer, timestamp };

    setTimeout(() => {
        remove(id);
    }, Date.now() - timestamp + ID_TTL);

    const potenID = {
        id,
        timestamp,
        ttl: ID_TTL,
    };
    return potenID;
}

function remove(id) {
    console.log("removing from potential queue: ", potentialQueue[id]);
    return delete potentialQueue[id];
}

function get(id) {
    if (potentialQueue[id]) {
        return potentialQueue[id];
    }
    return null;
}

function exists(id) {
    console.log(potentialQueue);
    if (potentialQueue[id]) {
        return true;
    }
    return false;
}

function setSocketID(potenID, socketID) {
    if (!potentialQueue[potenID]) {
        return false;
    }
    potentialQueue[potenID]["socketID"] = socketID;
    return true;
}

module.exports = { get, setSocketID, generateID, remove, exists };
