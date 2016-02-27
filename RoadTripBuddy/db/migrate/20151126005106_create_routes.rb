class CreateRoutes < ActiveRecord::Migration
  def change
    create_table :routes do |t|
      t.references :user, index: true, foreign_key: true
      t.references :start, index: true, foreign_key: true
      t.references :end,   index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
