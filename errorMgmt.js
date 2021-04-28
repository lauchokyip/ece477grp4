const universalEmitter = require("./universalEmitter.js");

const MAX_SIZE = 10;

const tempErrors = [];
const unauthorizedEntries = [];

function getFullTemp() {
    return tempErrors;
}

function getFullEntry() {
    return unauthorizedEntries;
}

function getTemp(index) {
    if (index < 0 || index >= MAX_SIZE || index >= tempErrors.length) {
        return null;
    }
    return tempErrors[index];
}

function getEntry(index) {
    if (index < 0 || index >= MAX_SIZE || index >= unauthorizedEntries.length) {
        return null;
    }
    return unauthorizedEntries[index];
}

function putTemp(temp) {
    var tempObj = createTempObj(temp);
    if (tempErrors.length == MAX_SIZE) {
        tempErrors.pop();
    }
    tempErrors.unshift(tempObj);
    universalEmitter.emit("statusChange");
}

function createTempObj(temp) {
    return { temp: temp, timestamp: Date.now() };
}

function removeTemp(index) {
    if (index < 0 || index >= tempErrors.length || index >= MAX_SIZE) {
        return;
    }
    tempErrors.splice(index, 1);
    universalEmitter.emit("statusChange");
}

function putEntry() {
    var entryObj = createEntryObj();
    if (unauthorizedEntries.length == MAX_SIZE) {
        unauthorizedEntries.pop();
    }
    unauthorizedEntries.unshift(entryObj);
    universalEmitter.emit("statusChange");
}

function createEntryObj() {
    return { timestamp: Date.now() };
}

function removeEntry(index) {
    if (index < 0 || index >= unauthorizedEntries.length || index >= MAX_SIZE) {
        return;
    }
    unauthorizedEntries.splice(index, 1);
    universalEmitter.emit("statusChange");
}

module.exports = {
    getEntry,
    getTemp,
    putEntry,
    putTemp,
    getFullEntry,
    getFullTemp,
    removeEntry,
    removeTemp,
};
