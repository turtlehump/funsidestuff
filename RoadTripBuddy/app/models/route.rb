class Route < ActiveRecord::Base
  belongs_to :user
  belongs_to :start
  belongs_to :end
  has_many :route_times
end
