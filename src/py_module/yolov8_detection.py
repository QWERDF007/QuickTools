from ultralytics.models.yolo.detect import DetectionPredictor
from ultralytics import YOLO

class Yolov8Detection:
    def __init__(self, model_path, imgsz, device):
        self.init_model(model_path, imgsz, device)
    
    def init_model(self, model_path, imgsz, device):
        self.model_path = model_path
        self.imgsz = imgsz
        self.device = device
        cfg = {'batch': 1, 'model': self.model_path, 'conf': 0.01, 'imgsz': self.imgsz, 
                'device': self.device,'save_txt': False, 'visualize': False, 'save': False, }
        self.model = DetectionPredictor(overrides=cfg)
        # self.model = YOLO(self.model_path)

    def detect(self, img, conf, iou):
        print(img.shape, flush=True)
        print('inferencing...', flush=True)
        self.model(img)
        print('inference over', flush=True)
        # self.model.args.conf = conf
        # self.model.args.iou = iou
        # print('inferencing...', flush=True)
        # result = self.model(img)
        # boxes = result[0].boxes
        # ret = {
        #     'status': 0,
        #     'msg': 'ok',
        #     'boxes': boxes.xyxy.tolist(),
        #     'cls': boxes.cls.tolist(),
        #     'conf': boxes.conf.tolist()
        # }
        # return ret
        