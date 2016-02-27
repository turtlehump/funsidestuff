class Start < ActiveRecord::Base
  belongs_to :user
  has_many :ends, through: :routes
  has_many :routes, dependent: :destroy
  #this (^) lines main functionality is the dependent: :destroy, not to create a @start.routes

  geocoded_by :address
  reverse_geocoded_by :latitude, :longitude
  after_validation :geocode, if: :address_changed?
  after_validation :reverse_geocode, if: :latitude_changed? or :longitude_changed?
  #^only changes if the latitude (the first one) is changed...
  #changing only the longitude (the second one) has no effect


#  after_validation :reverse_geocode, if: :longitude_changed?
#  after_validation :reverse_geocode, if: :latitude_changed?
  #if I separate them out into 2 lines then the 2nd line overwrites the first line
  #will only update when the 2nd one is modified. Changing only the first one has no effect

  validates :name, presence: true

end
