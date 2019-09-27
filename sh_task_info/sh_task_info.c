#include<linux/sched.h>
#include<linux/kernel.h>
#include<linux/fcntl.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/syscalls.h>
#include<asm/uaccess.h>
#include<linux/fs.h>
#include<linux/mm.h>
#include<linux/signal.h>
#include<linux/file.h>

asmlinkage long sys_sh_task_info(int user_pid, char *fname)
{
	int error = 0;
	struct task_struct *task;
	struct file *file;
	int fd;
	struct sched_entity se;
	struct thread_struct thr;
	mm_segment_t old_fs = get_fs();
	char data[3000];
	loff_t pos = 0;
	set_fs(KERNEL_DS);

	for_each_process(task)
	{
		if(task->pid == user_pid){
			fd = sys_open(fname,O_WRONLY|O_CREAT,0644);
			if(fd>=0){
				se = task->se;
				thr = task->thread;
				sprintf(data, "Name:  %s\n State:  %ld\n On_cpu:  %d\n Priority:  %d\n Static_Priority:  %d\n Normal_Priority:  %d\n Rt_Priority:  %u\n PID:  %d\n Tgid:  %d\n Blocked_sigset:  %d\n Real_blocked sigset:  %d\n Schedule_entity(on_rq):  %u\n Thread(sp):  %lu\n", task->comm, task->state, task->on_cpu, task->prio, task->static_prio, task->normal_prio, task->rt_priority, task->pid, task->tgid, task->blocked, task->real_blocked, se.on_rq, thr.sp);
				sys_write(fd,data,strlen(data));
				file = fget(fd);
				if(file){
					printk("%s",data);
					vfs_write(file,data,strlen(data),&pos);
					fput(file);
				}
				sys.close(fd);
			}

			else{
				//file not opened
				set_fs(old_fs);
				return 1;
			}
			set_fs(old_fs);
			error = 1 ;
			return 0;
		}
	}
 	if(error == 0){
		return -1; //invalid pid
	}

	return -2;  //whatever other error
}
