devices = {
	"transmitter" => {"staging" => "pro5v328", "production" => "leonardo"},
	"reciever" => {"staging" => "pro328", "production" => "leonardo"}
}

task :build, [:device, :scenario] do |t, args|
	Dir.chdir args.device do
		variant = devices[args.device][args.scenario]
		system "ano build -m #{variant}"
	end
end

task :upload, [:device, :scenario] do |t, args|
	Dir.chdir args.device do
		system "ano upload -m #{devices[args.device][args.scenario]}"
	end
end

task :serial, [:port] do |t, args|
	system "miniterm.py #{args.port} 19200"
end

