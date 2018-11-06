'use strict'
const express = require('express')
const page1 = require('./routes/page1')
const app = express()

app.set('views', './views')
app.set('view engine', 'pug')

app.get("/", function(req, res, next) {
    res.render('index')
})

app.use('/page1', page1)

const server = app.listen(3000, function(){
    console.log("Node.js is listening to PORT:" + server.address().port)
})

