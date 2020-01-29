### Try this

``` cpp

            std::error_code ec;
            if (std::filesystem::rename(candidatePath, rotatedPath, ec); ec)
            {
                LOG_ERROR(boost::format("Failed to rotate [%1%], reason: [%2%], code: [%3%]")
                    % candidatePath
                    % ec.message()
                    % ec.value());
            }

```