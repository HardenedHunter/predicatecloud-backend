const express = require('express');
const router = express.Router();
const _ = require('lodash');
const bcrypt = require('bcrypt');
const Joi = require('joi');
// const mailer = require('../utils/mailer');
const config = require('config');
const auth = require('../middleware/authentication');
const admin = require('../middleware/admin');
const { User, validateUser, userSchema } = require('../models/user');

//Registration
router.post('/', async (req, res) => {
  const { error } = validateUser(req.body);
  if (error) return res.status(400).send(error.details[0].message);

  let user = await User.findOne({ email: req.body.email });
  if (user) return res.status(409).send('User already registered.');

  user = new User(
    _.pick(req.body, ['name', 'email', 'password', 'isAdmin', 'oauthId'])
  );

  const salt = await bcrypt.genSalt(config.get('hashCycles'));
  user.password = await bcrypt.hash(user.password, salt);
  await user.save();

  //   await mailer.sendConfirmation(user);

  const token = user.generateAuthToken();
  res
    .header('x-auth-token', token)
    .header('access-control-expose-headers', 'x-auth-token')
    .send(_.pick(user, ['_id', 'name', 'email']));
});

//Changing user information.
router.put('/:id', auth, async (req, res) => {
  const { error } = Joi.validate(req.body, {
    name: userSchema.name,
    email: userSchema.email,
  });
  if (error) return res.status(400).send(error.details[0].message);

  if (req.params.id != req.user._id) return res.status(403).send('Forbidden.');

  const user = await User.findByIdAndUpdate(
    req.user._id,
    {
      $set: {
        name: req.body.name,
        email: req.body.email,
      },
    },
    { new: true }
  );

  const token = user.generateAuthToken();
  res.send(token);
});

//Deletes the user with the given id
router.delete('/:id', [auth, admin], async (req, res) => {
  const user = await User.findByIdAndRemove(req.params.id);
  if (!user) return res.status(404).send('The user was not found.');
  res.send(_.pick(user, ['_id', 'name', 'email']));
});

//Admin panel
router.get('/:id', [auth, admin], async (req, res) => {
  const user = await User.findById(req.params.id).select('-password');
  if (!user) return res.status(401).send('Incorrect user information.');
  res.send(user);
});

//Admin panel
router.get('/', [auth, admin], async (req, res) => {
  const users = await User.find().sort('name').select('-password');
  res.send(users);
});

module.exports = router;
