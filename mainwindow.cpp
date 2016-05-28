#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWindow>

/**
 * @brief MainWindow::MainWindow
 * @param camera
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    setMinimumHeight(180);
    setMinimumWidth(1024);
    setMaximumWidth(1024);
    setMaximumHeight(768);

    ui->setupUi(this);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    screensList = QApplication::screens();
    qDebug() << "screensList.size()" << screensList.size();

    graphicsScene = new GraphicsScene(ui->graphicsView);

    ui->graphicsView->setScene(graphicsScene);
    ui->startButton->setEnabled(false);
    ui->startButton->setToolTip("Генерация субпиксельной шахматной проекции");

    ui->actionReset->setEnabled(false);

    ui->camVendorIDBox->setDuplicatesEnabled(false);

    id_ = 0;

    sharedImageBuffer = new SharedImageBuffer();


    ui->streamWidget->setMouseTracking(true);
    ui->screenImage->setScaledContents(true);

    monitorsFound = false;
    camIDsList_.clear();
    gridActive = false;

    searchCameras();
    searchDisplays();

    initialStart_ = true;

    connect(ui->camVendorIDBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectCamera(int)));

    connect(ui->camVendorIDBox,SIGNAL(highlighted(int)),this,SLOT(updateConnectedCameras()));

    connect(ui->camResolBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateCameraSettings(int)));

    connect(ui->monVendorIDBox,SIGNAL(currentIndexChanged(int)),this,SLOT(showAllMonResolutions(int)));

    connect(ui->monVendorIDBox,SIGNAL(highlighted(int)),this,SLOT(updateConnectedDisplays()));

    captureThread_ = new CaptureThread(sharedImageBuffer,id_,camWidth_,camHeight_);
    streamThread_ = new StreamThread(sharedImageBuffer,id_);

    setupCamera();

    ratio_ = ((double)camWidth_ / (double) camHeight_);
    graphicsScene->setSceneRect(0,0,camWidth_,camHeight_);
    ui->graphicsView->setFixedSize(camWidth_,camHeight_);
    ratio_ = (( ceil(ratio_*100)/100.));
    qDebug() << "initial ratio="<<ratio_;
    graphicsScene->restartFrame((double)camWidth_/camHeight_,(double)camWidth_/camHeight_);

    int res = computeRatio(camWidth_,camHeight_);



    if (res == -1){
        qDebug() << "unknown screen resolution";
    }
    else {

        ui->startButton->setEnabled(true);
        dispWidth  =  getMonitorWidth();
        dispHeight  = getMonitorHeight();

        if ((dispWidth == 0 || dispHeight == 0) || (!monitorsFound)){
            QMessageBox::critical(0, tr("Ошибка"), tr("Не определен монитор"));
            return;
        }
    }
    selectCamera(id_);
    initialStart_ = false;
}

MainWindow::~MainWindow()
{
    stopProcessing();

    disconnect(ui->camVendorIDBox,SIGNAL(activated(int)),this,SLOT(showAllCamResolutions(int)));
    disconnect(ui->camVendorIDBox,SIGNAL(activated(int)),this,SLOT(selectCamera(int)));
    disconnect(ui->camVendorIDBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectCamera(int)));
    disconnect(ui->camVendorIDBox,SIGNAL(highlighted(int)),this,SLOT(updateConnectedCameras()));
    disconnect(ui->camResolBox,SIGNAL(activated(int)),this,SLOT(updateCameraSettings(int)));
    disconnect(ui->monVendorIDBox,SIGNAL(currentIndexChanged(int)),this,SLOT(showAllMonResolutions(int)));
    disconnect(ui->monVendorIDBox,SIGNAL(highlighted(int)),this,SLOT(updateConnectedDisplays()));

    delete graphicsScene;
    delete ui;
}



/**
 * @brief MainWindow::getMonitorWidth           Возвращает текущее установленное разрешение монитора - ширина
 * @return
 */
int MainWindow::getMonitorWidth(){
    QString widthStr = ui->monResolBox->currentText();
    QStringList resolList = widthStr.split("x");
    widthStr = resolList.first();

    int width = widthStr.toInt(0,10);
    return width;
}

/**
 * @brief MainWindow::getMonitorHeight          Возвращает текущее установленное разрешение монитора - высота
 * @return
 */
int MainWindow::getMonitorHeight(){
    QString heightStr = ui->monResolBox->currentText();
    QStringList resolList = heightStr.split("x");
    heightStr = resolList.at(1);
    qDebug() << "height = ";
    qDebug() << heightStr;
    int height = heightStr.toInt(0,10);
    return height;
}

int MainWindow::getMonitorBitrate()
{
    QString bitsStr = ui->monResolBox->currentText();
    QStringList resolList = bitsStr.split("x");
    bitsStr = resolList.at(2);
    qDebug() << "height = ";
    qDebug() << bitsStr;
    int bits = bitsStr.toInt(0,10);
    return bits;
}


/**
 * @brief MainWindow::showAllMonResolutions         Возвращает через WinApi все возможные разрешения монитора в
 *                                                  зависимости от номера монитора в системе, записывает разрешения в ComboBox
 *                                                  и отображает номер монитора в изображении
 * @param displayIndex
 */
void MainWindow::showAllMonResolutions(int displayIndex){
   // получение разрешения экрана по умолчанию
    displayIndex_ = displayIndex;
    QScreen *screen = screensList[displayIndex_];

    defaultDisplayHeight_ = screen->size().height();
    defaultDisplayWidth_ = screen->size().width();

    defaultDisplayBpp_ = screen->depth();


    qDebug() << "defaultDisplayWidth_=="<<defaultDisplayWidth_;
    qDebug() << "defaultDisplayHeight_=="<<defaultDisplayHeight_;
    qDebug() << "defaultBpp=" << defaultDisplayBpp_;

    ui->monResolBox->clear();
    qDebug() <<"displayIndex" << displayIndex;

    QString dispName = ui->monVendorIDBox->currentText();
    qDebug() << "dispName====" <<dispName;

    DEVMODE dm = { 0 };

    DISPLAY_DEVICE dd;

    dd.StateFlags = DISPLAY_DEVICE_ATTACHED_TO_DESKTOP;

    dd.cb = sizeof(dd);


   EnumDisplayDevicesW(0, displayIndex_,&dd,1);

        qDebug() << "iDevNum=" << displayIndex_;
        qDebug() << "DeviceName=" << QString::fromWCharArray(dd.DeviceName,32);
        qDebug() << "DeviceString=" << QString::fromWCharArray(dd.DeviceString,128);
        qDebug() << "dd.DeviceID=" << QString::fromWCharArray(dd.DeviceID,128);


         for( int iModeNum = 0; EnumDisplaySettingsW(dd.DeviceName, iModeNum,&dm) != 0; iModeNum++) {
                    //для совместимости с современными ОС не использовать 8-ми и 16-ти битные режимы мониторов

                if ( (dm.dmBitsPerPel == 8) || (dm.dmBitsPerPel == 16) ){
                    return;
                }

                   ui->monResolBox->addItem(QString::number(dm.dmPelsWidth,10) + "x" + QString::number(dm.dmPelsHeight,10)+"x"+ QString::number(dm.dmBitsPerPel,10));

            }

         switch (displayIndex){
            case 0:
                 ui->screenImage->setPixmap(QPixmap::fromImage(QImage(":/images/images/1pr.png")));
             break;
         case 1:
                 ui->screenImage->setPixmap(QPixmap::fromImage(QImage(":/images/images/2pr.png")));
             break;
         }

}
/* */

/**
 * @brief MainWindow::showAllCamResolutions         Возвращает  все возможные разрешения камеры в
 *                                                  зависимости от номера в системе и записывает разрешения
 *                                                  в ComboBox.
 * @param cameraIndex                               Номер камеры в ComboBox
 */
void MainWindow::showAllCamResolutions(int cameraIndex)
{
    qDebug() << "cameraResolutionsList_.size();" << cameraResolutionsList_.size();
    ui->camResolBox->clear();
    QList<QString> camResolList = cameraResolutionsList_.value(cameraIndex);

    foreach (QString resol,camResolList){

         ui->camResolBox->addItem(resol);         
    }
}
/**
 * @brief MainWindow::selectCamera                  Выбор камеры.
 * @param cameraIndex
 */
void MainWindow::selectCamera(int cameraIndex)
{
    //При первом запуске приложения
   if (initialStart_){

       graphicsScene->resetFrame();
        qDebug() << "initialStart_";
       id_ = cameraIndex;

       startProcessing(id_);
   }
   else {
        //При переключении между  камерами\разрешениями
       qDebug() << "non initialStart_";
       stopProcessing();
       id_ = cameraIndex;

       startProcessing(id_);
   }
}



/**
 * @brief MainWindow::updateConnectedCameras        Слот обновления списка подключенных камер
 */

void MainWindow::updateConnectedCameras()
{
    qDebug() << "test update";
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    QList<QString> newCamerasList = enum_cameras();
    CoUninitialize();

    if (newCamerasList.size() != camerasList_.size()){

        int cnt = 0;
        camerasList_ = newCamerasList;
        ui->camVendorIDBox->clear();
        foreach (QString it , camerasList_){

               QString camNum = QString::number(cnt,10);
               QString descr = it;
                  ui->camVendorIDBox->addItem(camNum+":"+descr);
                  cnt++;
        }
    }
}
/**
 * @brief MainWindow::updateConnectedDisplays       Слот обновления списка подключенных экранов
 */
void MainWindow::updateConnectedDisplays()
{
    QList<QString> newMonitorsList = getMonitorsList();

    if (newMonitorsList.size() != monitorsList_.size()){

        int cnt = 0;
        monitorsList_ = newMonitorsList;
        ui->monVendorIDBox->clear();
        foreach (QString it, monitorsList_){

               QString camNum = QString::number(cnt,10);
               QString descr = it;
                  ui->monVendorIDBox->addItem(camNum+":"+descr);
                  cnt++;
        }
        showAllMonResolutions( ui->monVendorIDBox->currentIndex() );
    }
}

void MainWindow::updateCameraSettings(int resIndex)
{
    stopProcessing();

    qDebug() <<"update!";
    int cameraWidth;
    int cameraHeight;
    int cameraFps;

    QString camWidthStr,camHeightStr,camFpsStr;
    QString camSettings = ui->camResolBox->currentText();
    if (camSettings.isEmpty()){

    }
    else {
        QStringList resolList = camSettings.split("x");

        camWidthStr = resolList.first();
        cameraWidth = camWidthStr.toInt();

         qDebug() << "camera Width" << cameraWidth;
        camHeightStr = resolList.at(1);
        cameraHeight = camHeightStr.toInt();

        camFpsStr = resolList.at(2);
        cameraFps = camFpsStr.toInt();

         qDebug() << "camera height" << cameraHeight;

            int res = computeRatio(cameraWidth,cameraHeight);
            qDebug() << "compute ratio finished";
            if (res == -1){
                qDebug() << "unknown screen resolution";
                return;
            }
            else {

                ui->startButton->setEnabled(true);

                dispWidth  =  getMonitorWidth();
                dispHeight  = getMonitorHeight();

                if ((dispWidth == 0 || dispHeight == 0) || (!monitorsFound)){
                    QMessageBox::critical(0, tr("Ошибка"), tr("Не определен монитор"));
                    return;
                }

                captureThread_->setCamWidth(camWidth_);
                captureThread_->setCamHeight(camHeight_);
                captureThread_->setDeviceNumber(id_);


                    startProcessing(id_);
            }
    }
}
/**
 * @brief MainWindow::saveSnapshots                 Слот вызывает снимки экрана и шахматки
 * @param cameraShot
 * @param gridShot
 */
void MainWindow::saveSnapshots(const QPixmap &cameraShot, const QPixmap &gridShot)
{
    stopProcessing();

    saveDialog = new SaveImageDialog(cameraShot,gridShot,0);
    int res = saveDialog->exec();

   if (res ==  QDialog::Accepted){
        qDebug() << "QDialog::Accepted";
        saveDialog->hide();
        delete saveDialog;
        startProcessing(id_);

    }

    else if (res == QDialog::Rejected){
        qDebug() << "QDialog::Rejected";
        saveDialog->hide();
        delete saveDialog;
        startProcessing(id_);

    }
}
/**
 * @brief MainWindow::resizeDisplayResolution       Возвращает разрешение экрана по умолчанию после закрытия шахматки
 */
void MainWindow::resizeDisplayResolution()
{

        DEVMODEW dm;
        LPDEVMODE pdm;
        long retval;


        DISPLAY_DEVICE dd;
        MONITORINFOEX mi;
        mi.cbSize = sizeof(mi);
        dd.cb = sizeof(dd);
        dd.StateFlags = DISPLAY_DEVICE_ATTACHED_TO_DESKTOP;
        dm.dmSize = sizeof(dm);
        EnumDisplayDevicesW(0, displayIndex_, &dd, 1); // перечисление всех дисплеев
        for( int iModeNum = 0; EnumDisplaySettingsW(dd.DeviceName, iModeNum,&dm) != 0; iModeNum++) {

        }
        dm.dmPelsWidth = defaultDisplayWidth_;
        dm.dmPelsHeight = defaultDisplayHeight_;
        dm.dmBitsPerPel = defaultDisplayBpp_;


         pdm = &dm;

         retval = ChangeDisplaySettingsEx(dd.DeviceName,pdm,NULL,CDS_UPDATEREGISTRY,NULL);

        if (retval != DISP_CHANGE_SUCCESSFUL ){
            qDebug() << "eror!";
        }

    disconnect(gridWidget,SIGNAL(gridDestroyed()),this,SLOT(resizeDisplayResolution()));
    ui->startButton->setEnabled(true);
    gridActive = false;
        delete gridWidget;
}

void MainWindow::processCamera()
{
    stopProcessing();
    startProcessing(id_);
}

/**/
/**
 * @brief MainWindow::getMonitorsList               Возвращает через API ОС все подключенные мониторы
 *
 */
 QList<QString> MainWindow::getMonitorsList(){

     QList<QString> monitorsList_;

        int cnt = 0;
     foreach (QScreen *screen, QGuiApplication::screens()) {
            monitorsList_.push_back(QString::number(cnt)+":"+screen->name());
            cnt++;
     }



    ui->monsFoundLbl->setText("Найдено мониторов: "+QString::number(cnt));

// // Для linux: c помощью shell скрипта xrandr --query извлекаются разрешения всех подключенных отображающих устройств
    return monitorsList_;
 }

 /**
  * @brief MainWindow::setupCamera                  Устанавливает настройки камеры: высоту, ширину, FPS
  */
 void MainWindow::setupCamera()
 {
     qDebug() << "setup camera";
     int cameraWidth;
     int cameraHeight;
     int cameraFps;

     QString camWidthStr,camHeightStr,camFpsStr;
     QString camSettings = ui->camResolBox->currentText();

     if (camSettings.isEmpty()){
         QMessageBox *error =  new QMessageBox("Ошибка!","Не установлены настройки камеры!",QMessageBox::Critical,QMessageBox::Ok,QMessageBox::Cancel,0);//  critical(this,);
         error->show();
     }
     else {
         QStringList resolList = camSettings.split("x");

         camWidthStr = resolList.first();
         cameraWidth = camWidthStr.toInt();

         camHeightStr = resolList.at(1);
         cameraHeight = camHeightStr.toInt();

         camFpsStr = resolList.at(2);
         cameraFps = camFpsStr.toInt();

          qDebug() << "camera height" << cameraHeight;

          Buffer<Mat> *imageBuffer = new Buffer<Mat>(100);
          sharedImageBuffer->add(id_, imageBuffer, false);

         camWidth_ = cameraWidth;
         camHeight_ = cameraHeight;
         camFps_ = cameraFps;

     }
 }


/**
 * @brief MainWindow::setCameraSize         Установка размеров viewport-a камеры
 * @param width
 * @param height
 */
void MainWindow::setCameraViewportSize(int width, int height)
{
    ui->streamWidget->setFixedSize(width,height);
}

/**
 * @brief MainWindow::getCamerasNames       Получение названий камер
 * @return
 */
QList<QString> MainWindow::getCamerasNames(){
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    QList<QString> camsList =  enum_cameras();
    CoUninitialize();

    return camsList;
}

/**
 * @brief MainWindow::getCameraResolutions  Получение разрешений камер
 */
void MainWindow::getCameraResolutions()
{
     CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
     int result = enum_devices();
     CoUninitialize();
     if (result  == 0)
        showAllCamResolutions(0);

}

/**
 * @brief MainWindow::searchCameras             Поиск всех подключенных камер. Вызывается при старте
 */
void MainWindow::searchCameras()
{
    camerasList_.clear();
    camerasList_ = getCamerasNames();
    int nCams = camerasList_.size();
    if (nCams){
//         QList<QString>::iterator it;
         int cnt = 0;
         foreach (QString it, camerasList_){

                QString camNum = QString::number(cnt,10);
                QString descr = it;
                   ui->camVendorIDBox->addItem(camNum+":"+descr);
                   cnt++;
         }

         ui->camsFoundLbl->setText("Найдено камер: "+QString::number(nCams));       
         ui->actionReset->setEnabled(true);
         getCameraResolutions();

    }
    else {

        QMessageBox::critical(0, tr("Ошибка"), tr("Нет подключенных камер!"));
        return;
    }
}

/**
    Функции работы с DirectShow
/*/
/**
 * @brief MainWindow::_FreeMediaType            Освобождает ресурсы в памяти, вызванные под DirectShow - фильтр
 * @param mt
 */
void MainWindow::_FreeMediaType(AM_MEDIA_TYPE& mt)
{
    if (mt.cbFormat != 0)
    {
        CoTaskMemFree((PVOID)mt.pbFormat);
        mt.cbFormat = 0;
        mt.pbFormat = NULL;
    }
    if (mt.pUnk != NULL)
    {
        // pUnk should not be used.
        mt.pUnk->Release();
        mt.pUnk = NULL;
    }
}

/**/
/**
 * @brief MainWindow::CamCaps                   Опрос параметров подключенных камер (разрешение, частота кадров)
 *                                              через фильтр DirectShow
 * @param pBaseFilter
 * @return
 */
HRESULT MainWindow::CamCaps(IBaseFilter *pBaseFilter)
{
    HRESULT hr = 0;
    vector<IPin*> pins;
    IEnumPins *EnumPins;
    pBaseFilter->EnumPins(&EnumPins);
    pins.clear();
    for(;;)
    {
        IPin *pin;
        hr=EnumPins->Next(1,&pin,NULL);
        if(hr!=S_OK){break;}
        pins.push_back(pin);
        qDebug() << "pins.size();" << pins.size();
        pin->Release();
    }
    EnumPins->Release();
    camIDsList_.push_back(pins.size());
    qDebug() << "camIDsList_camIDsList_=" << camIDsList_;

    PIN_INFO pInfo;
    qDebug() << "pins.size()==" << pins.size();
    for(int i=0;i<pins.size();i++)
    {

        pins[i]->QueryPinInfo(&pInfo);

        IEnumMediaTypes *emt=NULL;
        pins[i]->EnumMediaTypes(&emt);

        AM_MEDIA_TYPE *pmt;

        vector<SIZE> modes;

        int cnt =0;
        for(;;)
        {
            hr=emt->Next(1,&pmt,NULL);
            if(hr!=S_OK){break;}

            if ( (pmt->formattype == FORMAT_VideoInfo) &&

                (pmt->cbFormat >= sizeof(VIDEOINFOHEADER)) &&
                (pmt->pbFormat != NULL) )
            {
                VIDEOINFOHEADER *pVIH = (VIDEOINFOHEADER*)pmt->pbFormat;
                SIZE s;
                // размер кадра
                s.cy=pVIH->bmiHeader.biHeight;
                s.cx=pVIH->bmiHeader.biWidth;

                modes.push_back(s);

                REFERENCE_TIME t=pVIH->AvgTimePerFrame; // blocks (100ns) per frame
                int FPS=floor(10000000.0/static_cast<double>(t));
              //  printf("Size: x=%d\ty=%d\tFPS: %d\t bitrate: %ld\tbit/pixel:%ld\n",s.cx,s.cy,FPS,bitrate,bitcount);
                QString widthStr = QString::number(s.cx,10);

                QString heightStr = QString::number(s.cy,10);

                QString frsStr = QString::number(FPS,10);

                cameraResolutions_.push_back(widthStr+"x"+heightStr+"x"+frsStr);
            }

            cameraResolutionsList_.insert(cnt,cameraResolutions_);
            cnt++;

            _FreeMediaType(*pmt);
        }
        //----------------------------------------------------
        //
        //
        //
        //----------------------------------------------------
        modes.clear();
        emt->Release();
    }

    pins.clear();

    return S_OK;
}


/**
 * @brief MainWindow::process_filter            Вызов метода опроса камер через фильтр DirectShow
 * @param pBaseFilter
 */
void MainWindow::process_filter(IBaseFilter *pBaseFilter)
{
    CamCaps(pBaseFilter);
}
/* ) */
/**
 * @brief MainWindow::enum_devices              Перечислить медиа-устройства в системе (видео-камеры)
 * @return
 */
int MainWindow::enum_devices()
{
    HRESULT hr;
        qDebug() << "enum_devices()=";
        // Create the System Device Enumerator.
        ICreateDevEnum *pSysDevEnum = NULL;
        hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
            IID_ICreateDevEnum, (void **)&pSysDevEnum);
        if(FAILED(hr))
        {
            return hr;
        }

        IEnumMoniker *pEnumCat = NULL;
        hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);

        if(hr == S_OK)
            {
                // С помощю IMoniker (COM-интерфейс) обратится к .
                IMoniker *pMoniker = NULL;
                ULONG cFetched;
                while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
                {
                    IPropertyBag *pPropBag;
                    hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
                        (void **)&pPropBag);
                    if(SUCCEEDED(hr))
                    {
                        //Для получения имени устройства:
                        VARIANT varName;
                        VariantInit(&varName);
                        hr = pPropBag->Read(L"FriendlyName", &varName, 0);
                        VariantClear(&varName);

                        // С помощью IBaseFilter фильтра, происходит опрос характеристик камеры (разрешение, FPS):
                        IBaseFilter *pFilter;
                        hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,
                            (void**)&pFilter);

                        process_filter(pFilter);

                        //После выполнения опроса, интерфейсы нужно освободить, чтобы предотвратить утечку.
                        pPropBag->Release();
                    }
                    pMoniker->Release();
                }
                pEnumCat->Release();
        }
        pSysDevEnum->Release();

        return 0;
}

/**/
/**
 * @brief MainWindow::enum_cameras              Перечислить названия камер в системе с помощью COM-интерфейса
 * @return
 */
QList<QString> MainWindow::enum_cameras()
{
    HRESULT hr;

        // Create the System Device Enumerator.
        ICreateDevEnum *pSysDevEnum = NULL;
        hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
            IID_ICreateDevEnum, (void **)&pSysDevEnum);

        IEnumMoniker *pEnumCat = NULL;
        QList<QString> camsList;
        hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);
        if(hr == S_OK)
            {
                // Enumerate the monikers.
                IMoniker *pMoniker = NULL;
                ULONG cFetched;
                while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
                {
                    IPropertyBag *pPropBag;
                    hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
                        (void **)&pPropBag);
                    if(SUCCEEDED(hr))
                    {
                        VARIANT varName;
                        VariantInit(&varName);
                        hr = pPropBag->Read(L"FriendlyName", &varName, 0);
                        if (SUCCEEDED(hr))
                        {
                            //добавление в список названий камер
                            camsList.push_back(QString::fromStdWString(varName.bstrVal));

//                            wprintf(L"------------------> %s <------------------\n", varName.bstrVal);
                        }
                        //освобождение интерфейса
                        VariantClear(&varName);
                        pPropBag->Release();
                    }
                    pMoniker->Release();
                }
                pEnumCat->Release();
        }
        pSysDevEnum->Release();

        return camsList;
}

/**/
/**
 * @brief MainWindow::searchDisplays            Поиск всех подключенных дисплеев, мониторов или проекторов.
 *                                              Вызывается при старте
 */
void MainWindow::searchDisplays()
{
   QList<QString> monList =  getMonitorsList();

   monitorsList_  = monList;
//    QList<QString>::iterator it;
    foreach (QString it , monitorsList_){
        ui->monVendorIDBox->addItem(it);
    }
    ui->actionReset->setEnabled(true);

    showAllMonResolutions( ui->monVendorIDBox->currentIndex() );

    monitorsFound = true;
}

/**/
/**
 * @brief MainWindow::update                    Стрим камеры. В StreamWidget отображается видео
 * @param image
 */
void MainWindow::updateFrame(QImage image){

              ui->streamWidget->setData(image);              
}

/**/
/**
 * @brief MainWindow::startProcessing           Запускает стрим
 */
void MainWindow::startProcessing(int cameraIndex){


    id_ = cameraIndex;
    Buffer<Mat> *imageBuffer = new Buffer<Mat>(100);
    sharedImageBuffer->add(id_, imageBuffer, true);

    captureThread_->setDeviceNumber(id_);
    captureThread_->setCamWidth(camWidth_);
    captureThread_->setCamHeight(camHeight_);
    captureThread_->setCamFps(camFps_);

    streamThread_->setDeviceNumber(id_);


    if (captureThread_->connectToCamera()){

           streamThread_->restart();
           qDebug() << "camera started with id=" << id_;
           ui->streamWidget->clearViewport();
          connect(streamThread_, SIGNAL(newFrame(QImage)), this, SLOT(updateFrame(QImage)));
          captureThread_->start(QThread::NormalPriority);
          streamThread_->start(QThread::NormalPriority);

    }
    else {
        qDebug() << "could not connect to camera";
    }

}
/**/
/**
 * @brief MainWindow::stopProcessing            Останавливает стрим
 */
void MainWindow::stopProcessing(){

    if (captureThread_->disconnectCamera()){

         disconnect(streamThread_, SIGNAL(newFrame(QImage)), this, SLOT(updateFrame(QImage)));

    if(streamThread_->isRunning()){

         qDebug() << "stopping stream thread";
        streamThread_->stop();
        sharedImageBuffer->wakeAll();
        streamThread_->wait();
    }
    if(captureThread_->isRunning()){

        qDebug() << "stopping capture thread";
         captureThread_->stop();
         sharedImageBuffer->wakeAll();
         captureThread_->wait();
    }

    sharedImageBuffer->removeByDeviceNumber(id_);
     qDebug() << "camera stopped with id=" << id_;

    }
}

/**
 * @brief MainWindow::getFramePoints            Получение координат рамки, необходимой для
 *                                              выделения монитора
 * @return
 */

std::vector<Point2f> MainWindow::getFramePoints()
{
    QPointF *dp0 = graphicsScene->getDp0();
    QPointF *dp1 = graphicsScene->getDp1();
    QPointF *dp2 = graphicsScene->getDp2();
    QPointF *dp3 = graphicsScene->getDp3();

    Point2f pTL(( (float)dp0->x() ),( (float)(dp0->y() )));


       Point2f pTR(( (float)dp1->x() ),( (float)(dp1->y() )));


       Point2f pBR(( (float)dp2->x() ),( (float)(dp2->y() )));


       Point2f pBL( ( (float)dp3->x() ),( (float)(dp3->y() )) );
    std::vector<cv::Point2f> framePoints_;
    framePoints_.push_back(pTL);
    framePoints_.push_back(pTR);
    framePoints_.push_back(pBR);
    framePoints_.push_back(pBL);

     return framePoints_;
}

/**/
/**
 * @brief MainWindow::mouseDoubleClickEvent             Обработчик двойного клика - используется для
 *                                                      получения снимка шахматки
 * @param e
 */
void MainWindow::mouseDoubleClickEvent( QMouseEvent * e ){
    if (( e->button() == Qt::LeftButton ) && (gridActive))
    {
        saveSnapshots(ui->streamWidget->pixmap(),gridWidget->pixmap());
    }
    if (( e->button() == Qt::LeftButton ) && (!gridActive)) {
        qDebug() << "no dblclick";
        return;
    }
}
/**
 * @brief MainWindow::closeEvent                        Корректный выход из приложения
 */
void MainWindow::closeEvent(QCloseEvent *)
{
    qApp->exit(0);
}


/*  */
/**
 * @brief MainWindow::on_startButton_clicked            Слот кнопки генерации шахматки.
 *                                                      Может немного зависнуть на пару секунд при
 *                                                      большой разнице между выбранным и установленным разрешением экрана.
 */
void MainWindow::on_startButton_clicked()
{

    dispWidth  =  getMonitorWidth();
    dispHeight = getMonitorHeight();
    dispBpp = getMonitorBitrate();

   std::vector<Point2f>  framePoints_ = getFramePoints();

    gridWidget = new GridWidget(ui->streamWidget,0);
    gridWidget->setVisible(true);
    ui->startButton->setEnabled(false);


    DEVMODEW dm;
        LPDEVMODE pdm;
        long retval;

        DISPLAY_DEVICE dd;
        MONITORINFOEX mi;
        mi.cbSize = sizeof(mi);
        dd.cb = sizeof(dd);
        dd.StateFlags = DISPLAY_DEVICE_ATTACHED_TO_DESKTOP;
        dm.dmSize = sizeof(dm);

        EnumDisplayDevicesW(0, displayIndex_, &dd, 1); // перечисление всех дисплеев

        qDebug() << "ddDeviceName=" << QString::fromStdWString(dd.DeviceName);
         for( int iModeNum = 0; EnumDisplaySettingsW(dd.DeviceName, iModeNum,&dm) != 0; iModeNum++) {

         }
         dm.dmPelsWidth = dispWidth;
         dm.dmPelsHeight = dispHeight;
         dm.dmBitsPerPel = dispBpp;
           pdm = &dm;

         //изменение разрешения конкретного экрана, указанного в параметрах
         retval = ChangeDisplaySettingsEx(dd.DeviceName,pdm,NULL,CDS_UPDATEREGISTRY,NULL);
         if (retval != DISP_CHANGE_SUCCESSFUL ){
             qDebug() << "error!";
         }

         gridWidget->setFixedSize(dispWidth,dispHeight);
         gridWidget->initGrid(dispWidth,dispHeight,camWidth_,camHeight_,framePoints_);
         gridActive = true;

         QWindow *win = gridWidget->windowHandle();
         win->setScreen(screensList[ui->monVendorIDBox->currentIndex()]);
         connect(gridWidget,SIGNAL(spacePressed(const QPixmap & ,const QPixmap &)),this,SLOT(saveSnapshots(const QPixmap & ,const QPixmap &)));
         gridWidget->showFullScreen();

         connect(gridWidget,SIGNAL(gridDestroyed()),this,SLOT(resizeDisplayResolution()));
         framePoints_.clear();

}


/*Действия из меню*/


/**
 * @brief MainWindow::on_actionAbout_triggered          Действие вызывает окно "О программе"
 */
void MainWindow::on_actionAbout_triggered()
{
    aboutDialog = new AboutDialog();
    aboutDialog->show();
}
/**
 * @brief MainWindow::on_actionQuit_triggered           Действие завершает приложение
 */
void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}


/* - */
/**
 * @brief MainWindow::computeRatio                      Расчет соотношения сторон для масштабирования окна стрима.
 *                                                      Нужен для правильного масштабирования. Нужно проверить на как можно
 *                                                      большем количестве камер - есть особые разрешения камер, которые не
 *                                                      подходят под стандарты (VGA,SXGA,HD и т.д).
 * @param width
 * @param height
 * @return
 */
int MainWindow::computeRatio(int &width, int &height)
{
    int hd = HD_RATIO; // 16:9
    int vga = VGA_RATIO; // 4:3
    int sxga = SXGA_RATIO; // 5:4
    int special = SPEC_RATIO; //176x144 - особый случай
    int qhd = QHD_RATIO; //768x448 - особый случай
    int unknown = UNKNOWN_RATIO; //неизвестное разрешение
    double ratio =  ( (double) width / (double) height);   //соотношение нового разрешение
    ratio = (( ceil(ratio*100)/100.));

    qDebug() << "ratio="<<ratio;

    if (ratio == HD_RATIO_SIZE) {
        ui->streamWidget->clearViewport();
        if ((width < HD_WIDTH) || (height < HD_HEIGHT)){
             ui->streamWidget->setFixedSize(width,height);
             ui->graphicsView->setFixedSize(width,height);

        }
        else {
            ui->streamWidget->setFixedSize(HD_WIDTH,HD_HEIGHT);
            ui->graphicsView->setFixedSize(HD_WIDTH,HD_HEIGHT);
        }


        graphicsScene->clearFrame();

        graphicsScene->setSceneRect(0,0,width,height);

        graphicsScene->restartFrame((double)width / height,(double)width / height);
        camWidth_ = width;
        camHeight_ = height;

        graphicsScene->resetFrame();
        return hd;

        }
   else if (ratio == VGA_RATIO_SIZE){

        ui->streamWidget->clearViewport();
        if ((width < VGA_WIDTH) || (height < VGA_HEIGHT)){
             ui->streamWidget->setFixedSize(width,height);
             ui->graphicsView->setFixedSize(width,height);
        }
        else {
            ui->streamWidget->setFixedSize(VGA_WIDTH,VGA_HEIGHT);
            ui->graphicsView->setFixedSize(VGA_WIDTH,VGA_HEIGHT);

        }

        graphicsScene->clearFrame();
        graphicsScene->setSceneRect(0,0,width,height);
        graphicsScene->restartFrame((double)width / height,(double)width / height);

        camWidth_ = width;
        camHeight_ = height;

        graphicsScene->resetFrame();

        return vga;

    }
    else if (ratio == SPECIAL_RATIO_SIZE){
        ui->streamWidget->clearViewport();

         if ((width < SPECIAL_WIDTH) || (height < SPECIAL_HEIGHT)){
              ui->streamWidget->setFixedSize(width,height);
              ui->graphicsView->setFixedSize(width,height);
         }
         else {
             ui->streamWidget->setFixedSize(SPECIAL_WIDTH,SPECIAL_HEIGHT);
             ui->graphicsView->setFixedSize(SPECIAL_WIDTH,SPECIAL_HEIGHT);
         }

        graphicsScene->clearFrame();
        graphicsScene->setSceneRect(0,0,width,height);
        graphicsScene->restartFrame((double)width / height,(double)width / height);

        camWidth_ = width;
        camHeight_ = height;

        graphicsScene->resetFrame();
        qDebug() << "set special";
        return special;
    }

    else if (ratio == SXGA_RATIO_SIZE){
        ui->streamWidget->clearViewport();

         if ((width < SXGA_WIDTH) || (height < SXGA_HEIGHT)){
              ui->streamWidget->setFixedSize(width,height);
              ui->graphicsView->setFixedSize(width,height);
         }
         else {
             ui->streamWidget->setFixedSize(SXGA_WIDTH,SXGA_HEIGHT);
             ui->graphicsView->setFixedSize(SXGA_WIDTH,SXGA_HEIGHT);
         }

        graphicsScene->clearFrame();
        graphicsScene->setSceneRect(0,0,width,height);
        graphicsScene->restartFrame((double)width / height,(double)width / height);

        camWidth_ = width;
        camHeight_ = height;

        graphicsScene->resetFrame();
        return sxga;
    }
    else if (ratio == QHD_RATIO_SIZE){
        ui->streamWidget->clearViewport();

        if ((width < QHD_WIDTH) || (height < QHD_HEIGHT)){
             ui->streamWidget->setFixedSize(width,height);
             ui->graphicsView->setFixedSize(width,height);
        }
        else {
            ui->streamWidget->setFixedSize(QHD_WIDTH,QHD_HEIGHT);
            ui->graphicsView->setFixedSize(QHD_WIDTH,QHD_HEIGHT);
        }
        graphicsScene->clearFrame();
        graphicsScene->setSceneRect(0,0,width,height);
        graphicsScene->restartFrame((double)width / height,(double)width / height);

        camWidth_ = width;
        camHeight_ = height;

        graphicsScene->resetFrame();
        return qhd;
    }
         return unknown;

}




