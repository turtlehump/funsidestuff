class RoutesController < ApplicationController
  before_action :set_route, only: [:show, :destroy]

  # GET /routes
  def index
    @routes = Route.all.map { |r| r if r.user == current_user }.compact.sort_by { |r| r.start[:name] }
  end

  # GET /routes/1
  def show
    s = @route.start
    e = @route.end
    center_lat = (s.latitude + e.latitude) / 2
    center_long = (s.longitude + e.longitude) / 2
    @distance = @route.start.distance_to(@route.end).round(2)
    @direction = @route.start.bearing_to(@route.end)
    @direction = Geocoder::Calculations.compass_point(@direction)
    if @distance < 1.4
      zoom = 14
    elsif @distance < 3
      zoom = 13
    elsif @distance < 5
      zoom = 12
    elsif @distance < 10
      zoom = 11
    elsif @distance < 13
      zoom = 10
    elsif @distance < 20
      zoom = 9
    elsif @distance < 35
      zoom = 8
    elsif @distance < 70
      zoom = 7
    elsif @distance < 200
      zoom = 6
    elsif @distance < 200
      zoom = 5
    elsif @distance < 300
      zoom = 4
    elsif @distance < 450
      zoom = 3
    elsif @distance < 700
      zoom = 2
    end
    @map_str = "http://maps.google.com/maps/api/staticmap?size=450x300&sensor=false"
    @map_str = @map_str + "&zoom=#{zoom}"
    @map_str = @map_str + "&center=#{center_lat},#{center_long}"
    @map_str = @map_str + "&markers=color:green%7Clabel:S%7C#{s.latitude},#{s.longitude}"  #label S for start
    @map_str = @map_str + "&markers=color:red%7Clabel:E%7C#{e.latitude},#{e.longitude}"  #label E for end
    @map_str = @map_str + "&key=AIzaSyCqWQ3TJM2l6Kb-nxSRUURzLy4agP8-9YQ"

    @route_time = @route.route_times.new
  end

  # GET /routes/new
  def new
    @starts = Start.all.map { |s| s if s.user == current_user }.compact
    @ends = End.all.map { |e| e if e.user == current_user }.compact
    @route = Route.new
  end

  # POST /routes
  def create
    @route = Route.new(route_params)
    @route.user = current_user

    if @route.save
      redirect_to @route, notice: 'Route was successfully created.'
    else
      render :new
    end
  end

  # DELETE /routes/1
  def destroy
    @route.destroy
    redirect_to routes_url, notice: 'Route was successfully destroyed.'
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_route
      @route = Route.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def route_params
      params.require(:route).permit(:start_id, :end_id)
    end
end
