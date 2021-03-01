const customerHelper = require("./helper/customerHelper.js");

console.log(makeCust(5, 4));
// var id = 5;
// var hexID = ("000000" + id.toString(16)).slice(-6);
// console.log(hexID);

function makeCust(id, numPeople) {
    var hexID = ("00000000" + id.toString(16)).slice(
        -1 * (customerHelper.ID_BITLEN / 4)
    );
    console.log(hexID);
    console.log(customerHelper.checkID(hexID));
    return customerHelper.make(
        customerHelper.makePoten("trial", "trial@trail.com", numPeople),
        hexID
    );
}
