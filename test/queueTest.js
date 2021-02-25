
const app = require('../queue')
const assert = require('chai').assert


describe('Testing Queue', () => {
    it('Queue should be empty before adding anything', () => {
        assert.equal(app.queue.length, 0)
    })
    it('Delete one customer when the queue is empty',  () => {
        const ret = app.deleteCustomerFromQueue(10)
        assert.equal(ret, false)
    })
    it('Add one customer to the queue with 4 people',  () => {
        const ret = app.addCustomerToQueue( 0, 4)
        assert.equal(ret, 0)
        assert.equal(app.queue.length, 1)
    })
    it('Add same customer to the queue (nothing should change)',  () => {
        const ret = app.addCustomerToQueue( 0, 4)
        assert.equal(ret, 0)
        assert.equal(app.queue.length, 1)
    })
    it('Delete invalid customer',  () => {
        const ret = app.deleteCustomerFromQueue(10)
        assert.equal(ret, false)
    })
    it('Delete the customer on the queue',  () => {
        const ret = app.deleteCustomerFromQueue(0)
        assert.equal(ret, true)
        assert.equal(app.queue.length, 0)
    })
    it('Add two customers on the queue',  () => {
        const firstCustomerIndex = app.addCustomerToQueue( 0, 4)
        const secondCustomerIndex = app.addCustomerToQueue( 5, 4)
        assert.equal(firstCustomerIndex, 0)
        assert.equal(secondCustomerIndex, 1)
        assert.equal(app.queue.length, 2)
    })
    it('Add same two customers on the queue (nothing should change)',  () => {
        const firstCustomerIndex =  app.addCustomerToQueue( 0, 4)
        const secondCustomerIndex =  app.addCustomerToQueue( 5, 4)
        assert.equal(firstCustomerIndex, 0)
        assert.equal(secondCustomerIndex, 1)
        assert.equal(app.queue.length, 2)
    })
    it('Delete invalid customer',  () => {
        const ret = app.deleteCustomerFromQueue(5)
        assert.equal(ret, false)
    })
    it('Delete first customer from the queue',  () => {
        const ret = app.deleteCustomerFromQueue(0)
        assert.equal(ret,true)
        assert.equal(app.queue[0].id, 5)
        assert.equal(app.queue.length, 1)
    })
    it('Delete second customer on the queue',  () => {
        const ret = app.deleteCustomerFromQueue(0)
        assert.equal(ret, true)
        assert.equal(app.queue.length, 0)
    })
    it('Add three customers on the queue',  () => {
        const firstCustomerIndex = app.addCustomerToQueue( 0, 4)
        const secondCustomerIndex = app.addCustomerToQueue(4, 4)
        const thirdCustomerIndex = app.addCustomerToQueue(5, 4)
        assert.equal(firstCustomerIndex, 0)
        assert.equal(secondCustomerIndex, 1)
        assert.equal(thirdCustomerIndex, 2)
        assert.equal(app.queue.length, 3)
    })
    it('Delete the middle customer',  () => {
        const ret = app.deleteCustomerFromQueue(1)
        assert.equal(ret,true)
        assert.equal(app.queue[0].id, 0)
        assert.equal(app.queue[1].id, 5)
        assert.equal(app.queue.length, 2)


        //clean up
        app.deleteCustomerFromQueue(0)
        app.deleteCustomerFromQueue(0)
    })
    
})