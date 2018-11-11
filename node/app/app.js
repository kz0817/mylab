'use strict'
const express = require('express')
const cookieParser = require('cookie-parser')
const page1 = require('./routes/page1')
const app = express()

app.use(cookieParser())

app.set('views', './views')
app.set('view engine', 'pug')

app.get('/', function(req, res, next) {
  const sessionId = req.cookies.sessionId
  if (!sessionId)
    res.redirect('login');
  else
    res.render('index')
})

app.get('/login', function(req, res, next) {
    res.render('login')
})

app.get('/logout', function(req, res, next) {
  res.clearCookie('sessionId');
  res.redirect('/')
})


app.post('/login', function(req, res, next) {
  const expireTime = 3600 * 1000 // milli-sec
  res.cookie('sessionId', 'test-id')
  res.redirect('/')
})

app.use('/page1', page1)

const server = app.listen(3000, function(){
    console.log('Node.js is listening to PORT:' + server.address().port)
})

