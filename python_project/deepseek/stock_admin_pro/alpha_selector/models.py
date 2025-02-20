from django.db import models
from django.db import transaction
from datetime import datetime
from django.contrib.auth.models import User
from django.db.models import F

# Create your models here.
# 实现自选股票models

class HoldingModel(models.Model):
    """
    持仓
    """ 
    HODING = 1
    EMPTY = 2
    HOLDING_TYPE_CHOICES = [
        (HODING, '持仓'),
        (EMPTY, '空仓')
    ]

    id = models.AutoField(primary_key=True)
    holding_quantity = models.IntegerField(default=0, verbose_name="持仓数量")
    cost_price = models.DecimalField(max_digits=10, decimal_places=2, verbose_name="持仓成本")
    code = models.IntegerField(default=0, verbose_name="股票代码")
    name = models.CharField(max_length=20, verbose_name="股票名称")
    create_at = models.DateTimeField(auto_now_add=True, verbose_name="创建时间")
    user = models.ForeignKey(User, on_delete=models.CASCADE, verbose_name="用户")
    status = models.IntegerField(choices=HOLDING_TYPE_CHOICES, default=HODING)
    holding_at  = models.DateTimeField(verbose_name="持仓时间")
    class Meta:
        db_table = 'holding'
        verbose_name = '持仓'
        verbose_name_plural = '持仓管理'


class TransactionRecord(models.Model):
    """
    交易记录
    """
    BUY = 1
    SELL = 2

    TRANSACTION_TYPE_CHOICES = [
        (BUY, '买入'),
        (SELL, '卖出'),
    ]
    transaction_id = models.AutoField(primary_key=True)
    amount = models.DecimalField(max_digits=10, decimal_places=2, verbose_name="交易金额", editable=False)
    transaction_type = models.IntegerField(
        choices=TRANSACTION_TYPE_CHOICES,
        default=BUY,  # 默认值
    )
    stock_code = models.CharField(max_length=10, verbose_name="股票代码", default="000000")
    stock_name = models.CharField(max_length=50, verbose_name="股票名称", default="无")
    transaction_price = models.DecimalField(max_digits=10, decimal_places=2, verbose_name="交易价格")
    transaction_quantity = models.IntegerField(verbose_name="交易数量", default=0)
    timestamp = models.DateTimeField(auto_now_add=True, verbose_name="交易时间")
    user = models.ForeignKey(User, on_delete=models.CASCADE, verbose_name="用户")

    def save(self, *args, **kwargs):
        self.amount = self.transaction_price * self.transaction_quantity
        with transaction.atomic():

            # 先保存交易记录
            super().save(*args, **kwargs)
            # 再更新持仓
            holding = HoldingModel.objects.filter(user=self.user, code=self.stock_code).first()
            if holding:
                if self.transaction_type == self.BUY:
                    holding.holding_quantity = F('holding_quantity') + self.transaction_quantity
                    holding.cost_price = (F('cost_price') * F('holding_quantity') + self.transaction_price * self.transaction_quantity) / (F('holding_quantity') + self.transaction_quantity)
                else:
                    holding.holding_quantity -= self.transaction_quantity
                    if holding.holding_quantity <= 0:
                        holding.holding_quantity = 0
                        holding.cost_price = 0
                    elif holding.holding_quantity > 0:
                        holding.cost_price = (holding.cost_price * holding.holding_quantity - self.transaction_price * self.transaction_quantity) / (holding.holding_quantity - self.transaction_quantity)
                        holding.status = 2
                holding.save()
            else:
                HoldingModel.objects.create(
                    holding_quantity=self.transaction_quantity,
                    cost_price=self.transaction_price,
                    code=self.stock_code,
                    name=self.stock_name,
                    user=self.user,
                    status=1,
                    holding_at=datetime.now()
                )
    class Meta:
        db_table = 'transaction_record'
        verbose_name = '交易记录'
        verbose_name_plural = '交易记录'
    
    