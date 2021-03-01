const ID_BITLEN = 32;
const MAX_PEOPLE_IN_PARTY = 100;
const ALLOW_ERROR = false;

// The Formaet of the customer object
// customer = {
//     name: string of length > 0
//     email: string of length > 0
//     id: string of length == ID_BITLEN / 4
//     numPeople: int > 0 && < MAX_PEOPLE_IN_PARTY
// }

// The Formaet of the potenCustomer object
// potenCustomer = {
//     name: string of length > 0
//     email: string of length > 0
//     numPeople: int > 0 && < MAX_PEOPLE_IN_PARTY
// }

// const NAME_KEY = "name";
// const EMAIL_KEY = "email";
// const NUM_PEOPLE_KEY = "numPeople";
// const ID_KEY = "id";

//object that is exported
const customerHelper = {
    ID_BITLEN,

    makePoten: function (name, email, numPeople) {
        if (typeof numPeople == "string") {
            numPeople = parseInt(numPeople);
        }
        var potenCustomer = {
            name,
            email,
            numPeople,
        };

        var err = this.potenIsInvalid(potenCustomer);
        if (ALLOW_ERROR & err) throw err;

        return potenCustomer;
    },

    potenIsInvalid: function (potenCustomer) {
        var err = null;

        if (!isObj(potenCustomer)) {
            err = "Input is not a JSON object.";
            return false, err;
        }

        if (!this.checkEmail(potenCustomer.email)) {
            err = "Invalid email.";
            return false, err;
        }
        if (!this.checkName(potenCustomer.name)) {
            err = "Invalid name.";
            return false, err;
        }
        if (!this.checkNumPeople(potenCustomer.numPeople)) {
            err = "Invalid numPeople.";
            return false, err;
        }

        return true, null;
    },

    make: function (potenCustomer, id) {
        if (!isObj(potenCustomer)) {
            throw `input "potenCustomer" is not an JSON object.`;
        }
        var customer = potenCustomer;
        customer.id = id;

        var err = this.isInvalid(customer);
        if (ALLOW_ERROR & err) throw err;

        return customer;
    },

    isInvalid: function (customer) {
        var err = null;

        if (!isObj(customer)) {
            err = "Input is not a JSON object.";
            return err;
        }

        if (!this.checkID(customer.id)) {
            err = "Invalid id.";
            return err;
        }
        if (!this.checkEmail(customer.email)) {
            err = "Invalid email.";
            return err;
        }
        if (!this.checkName(customer.name)) {
            err = "Invalid name.";
            return err;
        }
        if (!this.checkNumPeople(customer.numPeople)) {
            err = "Invalid numPeople.";
            return err;
        }

        return err;
    },
    checkEmail: function (email) {
        // add proper email format check if there is a need
        return isString(email);
    },

    checkName: function (name) {
        return isString(name);
    },

    checkID: function (id) {
        if (!isString(id)) return false;
        if (id.length !== ID_BITLEN / 4) return false;

        return true;
    },

    checkNumPeople: function (num) {
        if (!Number.isInteger(num)) return false;
        if (num <= 0) return false;

        return true;
    },
};

function isObj(obj) {
    if (obj === null || obj === undefined) return false;
    if (typeof obj != "object") return false;

    return true;
}

function isString(str) {
    if (str === null || str === undefined) return false;
    if (typeof str != "string") return false;
    if (str.length === 0) return false;

    return true;
}

if (typeof module != "undefined") {
    module.exports = customerHelper;
}
