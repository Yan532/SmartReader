#include "myrecordc.h"

static void wav_init_header(WAVEHDR *fileheader)

{

    /* stolen from cdda2wav */

    int nBitsPerSample = 16;

    int channels = 1;

    int rate = 16000;

    unsigned long nBlockAlign = channels * ((nBitsPerSample + 7) / 8);

    unsigned long nAvgBytesPerSec = nBlockAlign * rate;

    unsigned long temp = /* data length */ 0 + sizeof(WAVEHDR) - sizeof(CHUNKHDR);

    fileheader->chkRiff.ckid = cpu_to_le32(FOURCC_RIFF);

    fileheader->fccWave = cpu_to_le32(FOURCC_WAVE);

    fileheader->chkFmt.ckid = cpu_to_le32(FOURCC_FMT);

    fileheader->chkFmt.dwSize = cpu_to_le32(16);

    fileheader->wFormatTag = cpu_to_le16(WAVE_FORMAT_PCM);

    fileheader->nChannels = cpu_to_le16(channels);

    fileheader->nSamplesPerSec = cpu_to_le32(rate);

    fileheader->nAvgBytesPerSec = cpu_to_le32(nAvgBytesPerSec);

    fileheader->nBlockAlign = cpu_to_le16(nBlockAlign);

    fileheader->wBitsPerSample = cpu_to_le16(nBitsPerSample);

    fileheader->chkData.ckid = cpu_to_le32(FOURCC_DATA);

    fileheader->chkRiff.dwSize = cpu_to_le32(temp);

    fileheader->chkData.dwSize = cpu_to_le32(0 /* data length */);

}

static void wav_start_write(FILE* fd, WAVEHDR *fileheader)

{

    wav_init_header(fileheader);

    fwrite(fileheader,1, sizeof(WAVEHDR), fd);

}

static void wav_stop_write(FILE* fd, WAVEHDR *fileheader, int wav_size)

{

    unsigned long temp = wav_size + sizeof(WAVEHDR) - sizeof(CHUNKHDR);

    fileheader->chkRiff.dwSize = cpu_to_le32(temp);

    fileheader->chkData.dwSize = cpu_to_le32(wav_size);

    fseek(fd,0,SEEK_SET);

    fwrite(fileheader,1, sizeof(WAVEHDR), fd);

}

int init_pcm_capture(snd_pcm_t **handle,snd_pcm_uframes_t *frame,unsigned int * val)

{

    //1.定义pcm句柄

    //snd_pcm_t *handle = NULL;

    snd_pcm_hw_params_t *params = NULL;

    //2.打开pcm设备---采集方式打开

    //(SND_PCM_STREAM_CAPTURE:打开模式)

    snd_pcm_open(handle, "hw:0,1", SND_PCM_STREAM_CAPTURE, 0);

    //3 分配参数空间， 设置默认参数

    snd_pcm_hw_params_alloca(&params);

    //分配空间

    snd_pcm_hw_params_any(*handle, params); //默认值

    //4.设置交错模式

    snd_pcm_hw_params_set_access(*handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    //5设置量化

    snd_pcm_hw_params_set_format(*handle, params, SND_PCM_FORMAT_S16_LE);

    //6设置通道数据

    snd_pcm_hw_params_set_channels(*handle, params, 1);

    //7设置采样率

    int rate = 16000;

    snd_pcm_hw_params_set_rate_near(*handle, params, &rate,0);

    //8把设置好的参数设置到pcm设备

    snd_pcm_hw_params(*handle,params);

    //9.分配一个周期空间

    //获取一个周期的帧数
    //snd_pcm_hw_params_get_period_size(params, frame, 0);

    *frame = 32;
    snd_pcm_hw_params_set_period_size_near(*handle,params, frame,0);

    /* Write the parameters to the driver */
    snd_pcm_hw_params(*handle, params);

    /* Use a buffer large enough to hold one period */
    snd_pcm_hw_params_get_period_size(params,frame, 0);
    /* We want to loop for 3 seconds */
    snd_pcm_hw_params_get_period_time(params,val, 0);


}

void func_record()

{
    static int counter = 0;
    char filename[100] = "/root/rec";
    char number[10];
    char extension[] = ".wav";
    // 点击按钮后的操作
    counter = 1;
    sprintf(number, "%d", counter);  // 将计数器转换为字符串
    strcat(filename, number);  // 拼接文件名和编号
    strcat(filename, extension);  // 拼接文件名和扩展名
    printf("New filename: %s\n", filename);

    WAVEHDR wavheader;

    int total_len = 0;

    int ret;

    long loops ;

    unsigned int val;

    snd_pcm_t *handle = NULL;

    snd_pcm_uframes_t frame = 0;

    init_pcm_capture(&handle,&frame,&val);

    int size = frame * 2;//计算一个周期所需要的存储空间

    unsigned char *buffer = malloc(size);//分配一个周期的空间

    FILE *fp = fopen(filename,"wb");

    wav_start_write(fp, &wavheader);

    //loops = 3000000 / val;
    while(flag)
    {
        //loops--;
        snd_pcm_readi(handle, buffer, frame);//采集一个周期数据

        ret = fwrite(buffer,1,size,fp);

        total_len += size;

        if(ret != size)

        {
            printf("size is %d\n",size);

            printf("short wrote:%d\n",size-ret);

            printf("fwrite data error\n");
        }
    }
    flag = 1;

    wav_stop_write(fp, &wavheader, total_len);

    //销毁pcm句柄

    snd_pcm_drain(handle);

    //关闭pcm设备

    snd_pcm_close(handle);

    //释放堆空间（存储一个周期数据）

    free(buffer);

    fclose(fp);
}



void func_exchangeflag()
{
    flag = !flag;
    printf("flag%d\n",flag);
}
