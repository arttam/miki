# journalctl

Source: https://linuxhandbook.com/journalctl-command/

systemd is the default on most of the major Linux distributions. One of the main features of systemd is the way it collects logs and the tools it gives for analyzing those logs.

In traditional SysVinit system, you have syslog that stores logs in plain text files. Reading and analyzing those files require the use of find, grep, cut and many other commands.

systemd collects logs from more sources than syslogs, keeps the journal logs in binary format and gives you a command line tool to read, analyze and manipulate the logs. This is more streamlined than the syslogs.

What is journald? What is journalctl?
journald is the daemon from systemd that collects the logs from various log sources like syslog.

journalctl is the command line tool that lets you interact with the journal logs.

With journalctl, you can read logs, monitor the logs in real time, filter the logs based on time, service, severity and other parameters.

In this tutorial, I'll show you how to use journalctl to for reading, monitoring and analyzing the logs in Linux.

Check if journal logs are enabled on your system
Some Linux distributions, specially the desktop ones, don't enable the journal logs by default.

The default location of journald logs is /var/log/journal directory. You should make sure that this directory exists. If not, create it yourself.

Next, in the /etc/systemd/journald.conf file make sure that the value Storage is set to either auto or persistent.

journald logs config settings
The journald.conf file shows the default values. So even if there is a # in front of the entries, it means those are the default settings being used. If you want to change anything, you remove the # from that line.

How to List Systemd Services in Linux [Beginner’s Guide]
Check what systemd services run on your Linux system in this tutorial.

Linux Handbook
Abhishek Prakash

Using journalctl commands
Let me show you some of the most basic yet useful examples of journalctl command.

Read and search through logs with journalctl
If you just type journalctl in the terminal, it will show the journal logs in chronological order.

journalctl
journalctl logs
journalctl uses less underneath to show you the logs. Which means you can use the same keys to move around the logs as you do with the less command.

If you don't remember that, here's a quick recall:

Key	Description
Arrow	Move by one line
Space	Move down one page
b	Move up one page
g	Go to the first line
G	Go to the last line
100g	Go to the 100th line
/string	Search for the string from current position
n/N	Go to the next or previous search match
q	Exit the logs
If you don't want the logs to be displayed in less-like viewing mode, you can use the --no-pager flag. This will display entire logs directly on the screen.

journalctl --no-pager
This is not very useful and it will flood your screen if you have a huge amount of logs.

Show logs in reverse chronological order
As you noticed, the logs are shown in chronological order. This means the oldest stored logs are displayed first.

If you want to see the recent logs first, you can display the journal logs in reverse order with the option -r:

journalctl -r
It still uses a less command like view. So, press q to exit the log viewing mode.

Display only N recent lines of journal logs
Instead of showing all logs, you can choose to display only a certain number of lines from log using the -n option.

For example, the command below will display most recent 25 lines of the logs:

journalctl -n 25
Show journal logs in real time
Viewing recent logs is one thing, if you want to see the logs in real time, you can use the -f option of journalctl command:

journalctl -f
Like the -f option of the tail command, this will display the logs in real time in the follow mode.

Use Ctrl+C command to exit the real time view.

Display logs in UTC time
By default, the journal logs are shown in the local time of your system. If your system's time is set to a time other than UTC and you want to see the logs in UTC, you can do that using the --utc flag.

journalctl --utc
Show only kernel messages with -k
The systemd journal accumulates logs from different sources. If you just want to see Linux kernel logs, you can use the option -k.

journalctl -k
journalctl kernel logs
Tip: Use sudo to see all journal logs
Systemd is protective about what kind of logs to show to which user.

It may show some logs but not all the logs if you are a regular user:

abhishek@linuxhandbook:~$ journalctl -u ssh
Hint: You are currently not seeing messages from other users and the system.
      Users in groups 'adm', 'systemd-journal' can see all messages.
      Pass -q to turn off this notice.
-- Logs begin at Mon 2020-06-22 12:05:47 UTC, end at Tue 2020-07-14 11:59:29 UTC. --
-- No entries --
If you want access to all the logs, you should use sudo if you are a sudo user:

sudo journalctl -u ssh
Show messages from a particular boot session
This is an excellent feature of jounrald. The journalctl command allows you to access logs belonging to a specific boot session using the option -b.

You can list all the boot sessions with --list-boots flag.

journalctl --list-boots
The output will show the boot sessions with the boot time and an integer assigned to the boot sessions:

  -5 513008ead8464c23aab732a2feed5277 Sun 2020-07-12 20:43:38 IST—Sun 2020-07-12 22:40:02 IST
  -4 caff16e3f46a4479b5287fb9e294f610 Mon 2020-07-13 07:36:04 IST—Mon 2020-07-13 19:13:44 IST
  -3 5665f41cc50a4dec9955efacc2596d68 Mon 2020-07-13 20:30:55 IST—Mon 2020-07-13 22:20:34 IST
  -2 c7d17407b0bd476a930af503f64b6006 Tue 2020-07-14 07:58:41 IST—Tue 2020-07-14 18:50:04 IST
  -1 7ab5e04518ec455abe0e2c86fdaa46fa Tue 2020-07-14 21:19:27 IST—Tue 2020-07-14 22:42:11 IST
   0 91856e86d4ee4e828717913deb288568 Wed 2020-07-15 08:11:51 IST—Wed 2020-07-15 17:14:10 IST
Boot session 0 is the current boot sessions. Boot session -1 is the last booted session and so on.

journalctl -b -2
Imagine trying to do this in the old syslog system!

You cannot only get the boot logs like what you see in /var/log/boot.log. However, the boot logs are always at the beginning of logs if you are in the boot session view.

Filter journal logs for a specific systemd service
Filtering is a strong point of journal logs. You can filter logs based on the systemd services.

journalctl -u service_name
For example, if you want to see logs generated by SSH, you can use it like this:

journalctl -u ssh
You'll need to know the systemd service name of course.

Filter logs for a certain time interval
This is another example of the string filtering capability of the journal logs. You can filter logs for a certain time period and there are various ways to do that.

You may use natural language to filter the logs. Terms like yesterday, today and tomorrow are recognized.

journalctl --since=yesterday --until=now
You can also specify date or date time combination:

journalctl --since "2020-07-10"
You can also specify a time period with the dates and time:

journalctl --since "2020-07-10 15:10:00" --until "2020-07-12"
Time starts at 00:00:00 and it determines the day and date.

You can also use relative time like -1h20min to specify 1 hour 20 minutes in the past.

Filter logs based on UID, GID and PID
If you are debugging an issue, you may want to check the logs for a certain process using its PID.

The journal logs can also be filtered on User ID (UID), Group ID (GID) and Process ID (PID). Below is an example:

journalctl _PID=1234
Tip: Combine more than one options for more tailored log viewing
You can combine several options to view the desired logs.

For example, if you want to see only SSH logs from yesterday in UTC timestamps, you can use:

sudo journalctl -u ssh --since=yesterday --utc
Another common usage is to filter logs based on boot sessions. If you want to see only the SSH logs in the current session, you can use:

sudo journalctl -u ssh -b0
Possibilities are endless and you can combine the options based on your need.

Using journalctl -xe for viewing last few logs
You'll often find people suggesting to use journalctl -xe command.

-e: Jump to the end of the journal logs
-x: Show extra information on the log entries (if available)
Some log entries have additional information that are not displayed in the normal log viewing. Using the -x option may display such information.

What you see as a single line like this:

Jul 09 16:33:40 itsfoss systemd[1]: Started Run anacron jobs.
It could display more information like this:

Jul 09 16:33:40 itsfoss systemd[1]: Started Run anacron jobs.
-- Subject: A start job for unit anacron.service has finished successfully
-- Defined-By: systemd
-- Support: http://www.ubuntu.com/support
-- 
-- A start job for unit anacron.service has finished successfully.
-- 
-- The job identifier is 3702.
The additional info helps explain the context of an error or log event and the possible solutions.

Show only errors in logs with journalctl
To show all the errors in the current session, you can use:

journalctl -p 3 -xb
-p 3 : filter logs for priority 3 (which is error)
-x : provides additional information on the log (if available)
b : since last boot (which is the current session)
You can also use other priority level to get debug, or warning or even critical level logs. This table lists all the priority levels.

Priority	Code
0	emerg
1	alert
2	crit
3	err
4	warning
5	notice
6	info
7	debug
You can also display logs for a range of severity. For example, if you want to see all the warning, notice and info logs from the current session, you can use:

journalctl -p 4..6 -b0
You could have also used warning..info in the above command instead of 4..6.

Check how much disk space logs are taking
The journald collects logs from various sources and it stores logs of various levels including debug logs. Trust me, while retaining logs help in analyzing and auditing, they can take considerable amount of disk space.

You can check how much disk space the journal logs are taking with this journalctl command:

journalctl --disk-usage
You might get a surprise (or a shock) when you see the output:

abhishek@linuxhandbook:~$ journalctl --disk-usage 
Archived and active journals take up 2.8G in the file system.