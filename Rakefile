task :build, [:device, :scenario] do |t, args|
	if args.device == "transmitter"
		Dir.chdir "transmitter" do
			if args.scenario == "production"
				system "ino build -m leonardo"
			elsif args.scenario == "staging"
				system "ino build -m uno"
			else
				raise "Scenario #{args.scenario} not recognized"
			end
		end
	elsif args.device == "reciever"
		Dir.chdir "reciever" do
			if args.scenario == "production"
				system "ino build -m leonardo"
			elsif args.scenario == "staging"
				system "ino build -m uno"
			else
				raise "Scenario #{args.scenario} not recognized"
			end
		end
	else
		raise "Device #{args.device} not recognized"
	end
end

task :upload, [:device, :scenario] do |t, args|
	if args.device == "transmitter"
		Dir.chdir "transmitter" do
			if args.scenario == "production"
				system "ino upload -m leonardo"
			elsif args.scenario == "staging"
				system "ino upload -m uno"
			else
				raise "Scenario #{args.scenario} not recognized"
			end
		end
	elsif args.device == "reciever"
		Dir.chdir "reciever" do
			if args.scenario == "production"
				system "ino upload -m leonardo"
			elsif args.scenario == "staging"
				system "ino upload -m uno"
			else
				raise "Scenario #{args.scenario} not recognized"
			end
		end
	else
		raise "Device #{args.device} not recognized"
	end
end
