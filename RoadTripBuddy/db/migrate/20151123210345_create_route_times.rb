class CreateRouteTimes < ActiveRecord::Migration
  def change
    create_table :route_times do |t|
      t.integer :minutes, default: 0
      t.integer :seconds, default: 0
      t.references :route, index: true, foreign_key: true
      t.references :user,  index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
