# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20151126005106) do

  create_table "ends", force: :cascade do |t|
    t.string   "name"
    t.string   "description"
    t.integer  "user_id"
    t.boolean  "private",     default: false
    t.integer  "elevation"
    t.string   "address"
    t.float    "longitude"
    t.float    "latitude"
    t.datetime "created_at",                  null: false
    t.datetime "updated_at",                  null: false
  end

  add_index "ends", ["user_id"], name: "index_ends_on_user_id"

  create_table "route_times", force: :cascade do |t|
    t.integer  "minutes",    default: 0
    t.integer  "seconds",    default: 0
    t.integer  "route_id"
    t.integer  "user_id"
    t.datetime "created_at",             null: false
    t.datetime "updated_at",             null: false
  end

  add_index "route_times", ["route_id"], name: "index_route_times_on_route_id"
  add_index "route_times", ["user_id"], name: "index_route_times_on_user_id"

  create_table "routes", force: :cascade do |t|
    t.integer  "user_id"
    t.integer  "start_id"
    t.integer  "end_id"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  add_index "routes", ["end_id"], name: "index_routes_on_end_id"
  add_index "routes", ["start_id"], name: "index_routes_on_start_id"
  add_index "routes", ["user_id"], name: "index_routes_on_user_id"

  create_table "starts", force: :cascade do |t|
    t.string   "name"
    t.string   "description"
    t.integer  "user_id"
    t.integer  "elevation"
    t.string   "address"
    t.float    "latitude"
    t.float    "longitude"
    t.datetime "created_at",  null: false
    t.datetime "updated_at",  null: false
  end

  add_index "starts", ["user_id"], name: "index_starts_on_user_id"

  create_table "users", force: :cascade do |t|
    t.string   "email",                  default: "",    null: false
    t.string   "encrypted_password",     default: "",    null: false
    t.string   "reset_password_token"
    t.datetime "reset_password_sent_at"
    t.datetime "remember_created_at"
    t.integer  "sign_in_count",          default: 0,     null: false
    t.datetime "current_sign_in_at"
    t.datetime "last_sign_in_at"
    t.string   "current_sign_in_ip"
    t.string   "last_sign_in_ip"
    t.boolean  "admin",                  default: false, null: false
    t.datetime "created_at",                             null: false
    t.datetime "updated_at",                             null: false
  end

  add_index "users", ["email"], name: "index_users_on_email", unique: true
  add_index "users", ["reset_password_token"], name: "index_users_on_reset_password_token", unique: true

end
