Miconcounter::App.controllers :count do
  get :index, :with => :id do
    return 404 if params[:id] != ENV['MICONCOUNTER_COUNT_KEY']

    logFile = ENV['MICONCOUNTER_COUNT_LOG']

    access_count = getCount()
    access_count = access_count + 1

    begin
      File::open(logFile, 'w') do |file|
        file.puts access_count
      end
    rescue
      logger.error 'Log file access error.'
      return "NG"
    end

    pushWebsocket("counter", "count_up", access_count, 'count failed.')

    "OK"
  end
end
