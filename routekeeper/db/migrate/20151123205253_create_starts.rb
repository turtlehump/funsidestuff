class CreateStarts < ActiveRecord::Migration
  def change
    create_table :starts do |t|
      t.string :name
      t.string :description
      t.references :user, index: true, foreign_key: true

      t.integer :elevation
      t.string :address
      t.float :latitude
      t.float :longitude

      t.timestamps null: false
    end
  end
end
