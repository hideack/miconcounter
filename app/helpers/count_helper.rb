# Helper methods defined here can be accessed in any controller or view in the application

Miconcounter::App.helpers do
  def pushWebsocket(ch, event, message, errMessage)
    Pusher.app_id = ENV['PUSHER_APP_ID'] 
    Pusher.key    = ENV['PUSHER_APP_KEY'] 
    Pusher.secret = ENV['PUSHER_APP_SECRET'] 

    EM.run do
      deferrable = Pusher[ch].trigger_async(event, message)
      deferrable.callback {EM.stop}
      deferrable.errback do |error|
        logger.warn errMessage
        EM.stop
      end
    end
  end
end
