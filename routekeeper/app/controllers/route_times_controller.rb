# This controller does not have a show function
# since route_times are shown by routes, it does not make sense to
# show a single route_time

class RouteTimesController < ApplicationController
  before_action :set_route_time, only: [:destroy]

  # GET routes/:route_id/route_times/new
  # Create a new route_time in the context of a Route object
  # that way the route_times's foreign key (:route_id) will be
  # initialized correctly.
  def new
    @route = Route.find params[:route_id]
    @route_time = @route.route_times.new
  end


  # POST routes:/:route_id/route_times
  # we need the route's key in the URL to make sure that someone
  # isn't trying to hack the id of the new route_time's route
  # rails ensures that the URL has not be tampered with
  def create
    @route = Route.find params[:route_id]
    @route_time = @route.route_times.new(route_time_params)
    @route_time.user = current_user

    if @route_time.save
      redirect_to route_url(@route) , notice: 'Time was successfully created.'
    else
      render :new
    end
  end

  # DELETE /route_times/1
  def destroy
    @route_time.destroy
    redirect_to route_url(@route_time.route) , notice: 'Time was successfully destroyed.'
  end

  private
    # Use callbacks to share common setup or constraints between actions
    def set_route_time
      @route_time = RouteTime.find(params[:id])
    end

    # Never trust parameters from the scary internet,
    # only allow the white list through.
    def route_time_params
      params.require(:route_time).permit(:minutes, :seconds)
    end
end

