class CreateEnds < ActiveRecord::Migration
  def change
    create_table :ends do |t|
      t.string :name
      t.string :description
      t.references :user, index: true, foreign_key: true
      t.boolean :private, default: :false
      t.integer :elevation
      t.string :address
      t.float :longitude
      t.float :latitude

      t.timestamps null: false
    end
  end
end
