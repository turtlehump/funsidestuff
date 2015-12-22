class End < ActiveRecord::Base
  belongs_to :user
  has_many :starts, through: :routes
  has_many :routes, dependent: :destroy
  #this (^) lines main functionality is the dependent: :destroy, not to create a @end.routes

  geocoded_by :address
  after_validation :geocode, if: :address_changed?

  validates :name, presence: true

end
