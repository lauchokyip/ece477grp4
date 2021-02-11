
const app = require('../queue')
const assert = require('chai').assert;


describe('Testing Queue Method', function() {
    it('queue should be empty', () => {
        assert.equal(app.queue.length, 0)
    });
    it('adding one customer to queue', () => {
        app.addCustomerToQueue("James", 1, 4, "james@example.org")
        assert.equal(app.queue.length, 1)
    });
    it('finding Bond index', () => {
        app.addCustomerToQueue("Bond", 2, 3, "james@example.org")
        let bondID = app.customerIndexOnQueue(2)
        assert.equal(bondID, 1)
    })

});