const jwt = require('jsonwebtoken');
const mongoose = require('mongoose');
const Schema = mongoose.Schema;
const Joi = require('joi');
const config = require('config');

const userSchema = new Schema({
  name: {
    type: String,
    required: true,
    minlength: 1,
    maxlength: 255,
  },
  email: {
    type: String,
    required: true,
    minlength: 5,
    maxlength: 255,
  },
  password: {
    type: String,
    required: true,
    minlength: 8,
    maxlength: 255,
  },
  isAdmin: {
    type: Boolean,
    default: false,
  },
  isConfirmed: {
    type: Boolean,
    default: false,
  },
});

userSchema.methods.generateAuthToken = function () {
  const token = jwt.sign(
    {
      _id: this._id,
      name: this.name,
      email: this.email,
      isAdmin: this.isAdmin,
      isConfirmed: this.isConfirmed,
    },
    config.get('jwtPrivateKey')
  );
  return token;
};

const joiSchema = {
  name: Joi.string().min(1).max(255).required(),
  email: Joi.string().min(5).max(255).email().required(),
  password: Joi.string().min(8).max(255).required(),
  isAdmin: Joi.boolean().default(false),
};

const validateUser = (user) => {
  return Joi.validate(user, joiSchema);
};

const User = mongoose.model('User', userSchema);
module.exports = { User, validateUser, userSchema: joiSchema };
