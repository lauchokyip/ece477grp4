const queue = [];

/**
 * customerIndexOnQueue will check if the customer is on Queue or not
 * return the array index if customer is found, otherwise return -1
 * @param {*} customerId
 */
function customerIndexOnQueue(customerId) {
    return queue.findIndex(
        (arrayElement) => arrayElement.customer.id === customerId
    );
}

/**
 *  addCustomerToQueue will add the customer to the queue given id,numOfPeople
 *  return the array index if customer is already on the queue, otherwise returns the index of the new added customer
 * @param {*} customer
 */
function addCustomerToQueue(customer) {
    const ret = customerIndexOnQueue(customer.id);
    if (ret !== -1) return ret;
    queue.push({
        customer,
    });
    return queue.length - 1;
}

/**
 * deleteCustomerFromQueue will delete customer from the queue given the array index
 * return false if customer is not on the queue or any error occur when perform deletion,
 * return true if the customer is deleted
 * @param {*} arrayIndex
 */
function deleteCustomerFromQueue(arrayIndex) {
    if (arrayIndex < 0 || arrayIndex + 1 > queue.length) return false;
    deletedObject = queue.splice(arrayIndex, 1);
    if (deletedObject == undefined) return false;
    return true;
}

module.exports = {
    customerIndexOnQueue,
    addCustomerToQueue,
    deleteCustomerFromQueue,
};
