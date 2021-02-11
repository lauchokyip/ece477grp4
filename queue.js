
//     queue:[
//         {
//             name:
//             id:
//             #peopleInParty:
//             email:
//         },
//         {},

//     ],
//     numPeople:,


const queue = []
const capacity = 50 // capacity can be changed

function customerIndexOnQueue(customerId) {
    if (isCustomerOnQueue(customerId) == false)
        return -1;
    return queue.findIndex((arrayElement) => arrayElement.id === customerId )
}

function addCustomerToQueue(_name, _id, _numOfPeople, _email) {
    queue.push({
        name:_name, 
        id:_id, 
        numOfPeople:_numOfPeople,
        email : _email,
    });
}

function isCustomerOnQueue(customerId) {
    let ret = queue.find( o => o.id === customerId);
    return (ret === undefined) ? false : true 
}

module.exports = {
    queue,
    addCustomerToQueue,
    customerIndexOnQueue,
};

